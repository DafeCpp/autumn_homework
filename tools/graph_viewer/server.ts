/** Read-only Codespaces server. Node 24 runs TypeScript directly; no runtime packages. */
import { createServer } from 'node:http';
import { open, realpath, readdir, readFile } from 'node:fs/promises';
import { resolve, relative, isAbsolute, join, dirname, basename } from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';
import { parseArgs } from 'node:util';
import type { ServerResponse } from 'node:http';

const HERE = dirname(fileURLToPath(import.meta.url));
export const REPO = resolve(HERE, '../..');
const STATIC = join(HERE, 'static');
const MAX_FILE = 9 * 1024 * 1024;
export interface Edge { id: number; from: number; to: number }
export interface Graph { n: number; first: number; directed: boolean; edges: Edge[] }
interface Task { folder: string; directed: boolean; first: number; label: string }
interface Case { id: string; task: string; name: string; label: string }
export type Event =
  | {type: 'node' | 'edge'; id: number; state: string}
  | {type: 'value'; id: number; name: string; value: number}
  | {type: 'step'; message: string};
const TASKS: Record<string, Task> = {
  bfs_visualization: {folder: 'examples/bfs_visualization', directed: false, first: 1, label: 'BFS: расстояния от вершины 1'},
  task_01: {folder: 'task_01', directed: true, first: 0, label: 'Зависимости: a → b означает «a зависит от b»'},
  task_02: {folder: 'task_02', directed: true, first: 1, label: 'Односторонние дороги'},
  task_03: {folder: 'task_03', directed: false, first: 1, label: 'Уязвимые места сети'},
};
function contained(root: string, path: string) {
  const rel = relative(root, path);
  return rel !== '..' && !rel.startsWith('..' + (process.platform === 'win32' ? '\\' : '/')) && !isAbsolute(rel);
}
function missing(error: unknown) { return (error as NodeJS.ErrnoException).code === 'ENOENT'; }
export async function readBounded(path: string, root: string, limit = MAX_FILE): Promise<string> {
  const [actual, base] = await Promise.all([realpath(path), realpath(root)]);
  if (!contained(base, actual)) throw Error('Файл находится за пределами разрешённого каталога');
  const file = await open(actual, 'r');
  try {
    if (!(await file.stat()).isFile()) throw Error('Ожидается обычный файл');
    const buffer = Buffer.alloc(limit + 1);
    let offset = 0;
    while (offset < buffer.length) {
      const {bytesRead} = await file.read(buffer, offset, buffer.length-offset, offset);
      if (!bytesRead) break;
      offset += bytesRead;
    }
    if (offset > limit) throw Error('Файл слишком большой для просмотра');
    return new TextDecoder('utf-8', {fatal: true}).decode(buffer.subarray(0, offset));
  } finally { await file.close(); }
}
export async function catalog(repo = REPO): Promise<Case[]> {
  const result: Case[] = [];
  for (const [task, config] of Object.entries(TASKS)) {
    for (const folder of [join(config.folder, 'tests'), config.folder]) {
      let names: string[];
      try { names = await readdir(join(repo, folder)); } catch(error) { if(missing(error)) continue; throw error; }
      for (const name of names.sort()) {
        if (!name.endsWith('.in')) continue;
        const path = join(repo, folder, name);
        if (!contained(await realpath(repo), await realpath(path))) continue;
        result.push({id: [folder.replaceAll('\\', '/'), name].join('/'), task, name: basename(name, '.in'), label: config.label});
      }
    }
  }
  return result;
}
export function parseGraph(text: string, directed: boolean, first: number): Graph {
  const tokens = text.trim().split(/\s+/);
  if (tokens.length < 2 || tokens.some(x => !/^-?\d+$/.test(x))) throw Error('Ожидаются целые числа: n m и пары вершин');
  const values = tokens.map(Number), [n, m] = values;
  if (!Number.isSafeInteger(n) || !Number.isSafeInteger(m) || n < 1 || n > 500 || m < 0 || m > 2000) {
    throw Error('Для просмотра выберите небольшой тест: до 500 вершин и 2000 рёбер');
  }
  if (values.length !== 2+2*m) throw Error('Число рёбер не соответствует m');
  const edges: Edge[] = [];
  for (let id = 0; id < m; id++) {
    const from = values[2+2*id], to = values[3+2*id];
    if (!Number.isSafeInteger(from) || !Number.isSafeInteger(to) || from < first || from >= first+n || to < first || to >= first+n) throw Error('Номер вершины вне диапазона');
    edges.push({id, from, to});
  }
  return {n, first, directed, edges};
}
export function parseTrace(text: string, graph: Graph): {events: Event[]; warnings: string[]} {
  const lines = text.split('\n');
  const tail = lines.pop();
  const warnings = tail ? ['Последняя строка ещё не записана полностью'] : [];
  if (lines.length > 50002) throw Error('Слишком много событий');
  const events: Event[] = [], recorded: Edge[] = [];
  let graphSeen = false, last = '', sealed = false;
  const states = new Set(['idle', 'queued', 'active', 'done', 'tree', 'bridge', 'cut']);
  for (const [index, line] of lines.entries()) {
    const e: unknown = JSON.parse(line);
    if (!e || typeof e !== 'object' || Array.isArray(e)) throw Error('Событие должно быть объектом');
    const event = e as Record<string, unknown>;
    if (sealed) throw Error('События после завершения записи');
    if (index === 0 && (event.type !== 'header' || event.version !== 1)) throw Error('Неизвестная версия записи');
    switch (event.type) {
      case 'header':
        if (index !== 0) throw Error('Повторный заголовок записи');
        break;
      case 'graph':
        if (graphSeen || event.n !== graph.n || event.first !== graph.first || event.directed !== graph.directed) throw Error('Запись не соответствует графу теста');
        graphSeen = true;
        break;
      case 'add_edge':
        if (!graphSeen || events.length) throw Error('Рёбра должны быть записаны до шагов');
        recorded.push({id: event.id as number, from: event.from as number, to: event.to as number});
        break;
      case 'node': case 'edge': case 'value': {
        if (!graphSeen || !Number.isInteger(event.id)) throw Error('Некорректный идентификатор');
        const id = event.id as number;
        if (event.type === 'edge' ? id < 0 || id >= graph.edges.length : id < graph.first || id >= graph.first+graph.n) throw Error('Событие ссылается на неизвестный элемент');
        if (event.type === 'value') {
          if (typeof event.name !== 'string' || typeof event.value !== 'number' || !Number.isFinite(event.value)) throw Error('Неверное значение переменной');
          events.push({type: 'value', id, name: event.name, value: event.value});
        } else {
          if (typeof event.state !== 'string' || !states.has(event.state)) throw Error('Неизвестное состояние');
          events.push({type: event.type, id, state: event.state});
        }
        break;
      }
      case 'step':
        if (!graphSeen || typeof event.message !== 'string') throw Error('Нет подписи шага');
        events.push({type: 'step', message: event.message});
        break;
      case 'end': case 'truncated': sealed = true; break;
      default: throw Error('Неизвестное событие');
    }
    last = String(event.type);
  }
  if (!graphSeen || JSON.stringify(recorded) !== JSON.stringify(graph.edges)) throw Error('Запись графа неполная или относится к другому тесту');
  if (last !== 'end') warnings.push(last === 'truncated' ? 'Запись ограничена по размеру' : 'Запись ещё идёт или выполнение прервано');
  return {events, warnings};
}
export async function caseData(id: string, repo = REPO, traces = join(repo, '.graph-traces')) {
  const test = (await catalog(repo)).find(c => c.id === id);
  if (!test) throw Error('Неизвестный тест');
  const config = TASKS[test.task], path = join(repo, id);
  const input = await readBounded(path, repo);
  const graph = parseGraph(input, config.directed, config.first);
  async function optional(file: string) {
    try { return await readBounded(file, repo); } catch(error) { if(missing(error)) return ''; throw error; }
  }
  const [expected, actual] = await Promise.all([optional(path.replace(/\.in$/, '.out')), optional(path.replace(/\.in$/, '.out.actual'))]);
  let trace: {events: Event[]; warnings: string[]} = {events: [], warnings: []};
  try { trace = parseTrace(await readBounded(join(traces, test.task, test.name+'.jsonl'), traces), graph); }
  catch(error) { if (!missing(error)) trace.warnings.push('Не удалось открыть запись: '+(error as Error).message); }
  return {...test, input, graph, expected, actual, ...trace};
}
function send(response: ServerResponse, body: string | Buffer, type: string, status = 200) {
  response.writeHead(status, {
    'Content-Type': type+'; charset=utf-8', 'Content-Length': Buffer.byteLength(body),
    'Cache-Control': 'no-store', 'X-Content-Type-Options': 'nosniff',
    'Content-Security-Policy': "default-src 'self'; script-src 'self'; style-src 'self'; connect-src 'self'; object-src 'none'",
  });
  response.end(body);
}
export function viewerServer(repo = REPO, traces = join(repo, '.graph-traces'), preferredTask?: string) {
  return createServer(async (request, response) => {
    if (request.method !== 'GET') { send(response, 'Method not allowed', 'text/plain', 405); return; }
    try {
      const url = new URL(request.url || '/', 'http://localhost');
      if (url.pathname === '/api/cases') {
        const cases = await catalog(repo);
        if (preferredTask) cases.sort((a, b) => Number(b.task === preferredTask) - Number(a.task === preferredTask));
        send(response, JSON.stringify(cases), 'application/json');
      }
      else if (url.pathname === '/api/case') send(response, JSON.stringify(await caseData(url.searchParams.get('id') || '', repo, traces)), 'application/json');
      else {
        const files: Record<string, [string, string]> = {'/': ['index.html','text/html'], '/app.js': ['app.js','text/javascript'], '/model.js': ['model.js','text/javascript'], '/style.css': ['style.css','text/css'], '/favicon.svg': ['favicon.svg','image/svg+xml']};
        const entry = files[url.pathname];
        if (!entry) { send(response, 'Not found', 'text/plain', 404); return; }
        send(response, await readFile(join(STATIC, entry[0])), entry[1]);
      }
    } catch(error) { send(response, JSON.stringify({error: (error as Error).message}), 'application/json', 400); }
  });
}
if (process.argv[1] && import.meta.url === pathToFileURL(resolve(process.argv[1])).href) {
  const {values} = parseArgs({options: {port: {type:'string',default:'8765'}, host: {type:'string',default:'127.0.0.1'}, 'trace-dir': {type:'string',default:join(REPO,'.graph-traces')}}});
  const port = Number(values.port);
  if (!Number.isInteger(port) || port < 1 || port > 65535) throw Error('Invalid port');
  const server = viewerServer(REPO, resolve(values['trace-dir']!));
  server.on('error', error => { console.error(error.message); process.exitCode = 1; });
  server.listen(port, values.host, () => console.log(`Графы: http://localhost:${port} — Codespaces: Ports → Open in Browser`));
}
