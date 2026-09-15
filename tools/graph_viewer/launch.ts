/** One-command build, recording and viewing; no npm installation required. */
import { spawnSync } from 'node:child_process';
import { join } from 'node:path';
import { parseArgs } from 'node:util';
import { REPO, viewerServer } from './server.ts';

function run(command: string, args: string[], allowFailedTests = false): number {
  const result = spawnSync(command, args, {cwd: REPO, stdio: 'inherit'});
  if (result.error) throw Error(`Не удалось запустить ${command}: ${result.error.message}`);
  if (result.signal) throw Error(`${command} остановлен сигналом ${result.signal}`);
  const code = result.status ?? 1;
  if (code !== 0 && !(allowFailedTests && code === 1)) {
    throw Error(`${command} завершился с кодом ${code}`);
  }
  return code;
}

async function main() {
  const {values, positionals} = parseArgs({allowPositionals: true, options: {
    task: {type: 'string'},
    port: {type: 'string', default: '8765'},
    'prepare-only': {type: 'boolean', default: false},
    help: {type: 'boolean', short: 'h'},
  }});
  if (values.help) {
    console.log(`Запуск из корня репозитория:
  ./visualize                 Собрать BFS, записать шаги и открыть просмотрщик
  ./visualize task_03         Собрать своё решение и открыть его тесты
  ./visualize --port 8766     Использовать другой порт
  ./visualize --prepare-only Собрать и записать шаги без запуска сервера

В Codespaces: Ports → 8765 → Open in Browser. Остановка: Ctrl+C.
Для просмотра без сборки: npm --prefix tools/graph_viewer start`);
    return;
  }
  const port = Number(values.port);
  if (!Number.isInteger(port) || port < 1 || port > 65535) throw Error('Порт должен быть числом от 1 до 65535');
  if (positionals.length > 1 || (positionals.length && values.task)) {
    throw Error('Укажите одну задачу: ./visualize task_03');
  }
  const task = values.task || positionals[0];
  if (task && !['task_01', 'task_02', 'task_03'].includes(task)) {
    throw Error('Просмотрщик поддерживает task_01, task_02 или task_03. Без имени задачи запускается BFS.');
  }
  for (const command of ['cmake', 'python3']) {
    const result = spawnSync(command, ['--version'], {stdio: 'ignore'});
    if (result.error || result.status !== 0) throw Error(`Не найден ${command}. В Codespaces выполните Rebuild Container.`);
  }
  // Keep standalone-example and whole-project CMake caches separate from each
  // other and from the student's build/ directory and IDE settings.
  const buildRoot = join(REPO, '.graph-build', task ? 'project' : 'demo');
  const source = task ? REPO : join(REPO, 'examples/bfs_visualization');
  const build = task ? buildRoot : join(buildRoot, 'examples/bfs_visualization');
  console.log(`\n1/3 · Собираем ${task || 'учебный BFS'}…`);
  run('cmake', ['-S', source, '-B', build]);
  run('cmake', ['--build', build, '--target', task || 'bfs_visualization', '--parallel', '2']);
  console.log('\n2/3 · Проверяем тесты и записываем шаги…');
  const testCode = run('python3', [join(REPO, 'scripts/run_cases.py'),
    ...(task ? ['--tasks', task] : ['--example', 'bfs_visualization']),
    '--build-dir', buildRoot, '--trace-dir', join(REPO, '.graph-traces'), '--save-actual'], true);
  if (values['prepare-only']) {
    process.exitCode = testCode;
    console.log('\nЗаписи подготовлены.');
    return;
  }
  if (testCode) console.log('\nЕсть непройденные тесты. Их графы и записанные шаги можно изучить в просмотрщике.');
  const server = viewerServer(REPO, join(REPO, '.graph-traces'), task);
  await new Promise<void>((done, reject) => {
    server.once('error', reject);
    server.listen(port, '127.0.0.1', () => done());
  });
  console.log(`\n3/3 · Просмотрщик готов: http://localhost:${port}
В Codespaces: Ports → ${port} → Open in Browser. Остановка: Ctrl+C.
После изменения решения остановите просмотрщик и повторите эту же команду.`);
  const close = () => server.close(() => process.exit(0));
  process.once('SIGINT', close);
  process.once('SIGTERM', close);
}

main().catch((error: NodeJS.ErrnoException) => {
  console.error(error.code === 'EADDRINUSE'
    ? 'Порт занят. Остановите предыдущий просмотрщик или запустите ./visualize --port 8766.'
    : error.message);
  process.exitCode = 1;
});
