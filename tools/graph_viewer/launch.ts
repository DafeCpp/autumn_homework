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
  ./visualize scc             Косарайю и Тарьян: собрать оба примера и записать шаги
  ./visualize kosaraju        Только Косарайю (также: tarjan)
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
  if (task && !['task_01', 'task_02', 'task_03', 'scc', 'kosaraju', 'tarjan'].includes(task)) {
    throw Error('Поддерживаются task_01–03, scc, kosaraju, tarjan. Без имени запускается BFS.');
  }
  for (const command of ['cmake', 'python3']) {
    const result = spawnSync(command, ['--version'], {stdio: 'ignore'});
    if (result.error || result.status !== 0) throw Error(`Не найден ${command}. В Codespaces выполните Rebuild Container.`);
  }
  const projectTask = task?.startsWith('task_') ? task : undefined;
  const examples = task === 'scc' ? ['kosaraju_visualization', 'tarjan_visualization']
    : [task === 'kosaraju' || task === 'tarjan' ? task + '_visualization' : 'bfs_visualization'];
  let testCode = 0;
  for (const target of projectTask ? [projectTask] : examples) {
    // Separate CMake caches from students' IDE builds and other examples.
    const buildRoot = join(REPO, '.graph-build', projectTask ? 'project' : 'demo');
    const source = projectTask ? REPO : join(REPO, 'examples', target);
    const build = projectTask ? buildRoot : join(buildRoot, 'examples', target);
    console.log(`\n1/3 · Собираем ${target}…`);
    run('cmake', ['-S', source, '-B', build]);
    run('cmake', ['--build', build, '--target', target, target + '_tests', '--parallel', '2']);
    console.log('\n2/3 · Проверяем тесты и записываем шаги…');
    const unitCode = run(join(build, projectTask || '', target + '_tests'), [], true);
    const fileCode = run('python3', [join(REPO, 'scripts/run_cases.py'),
      ...(projectTask ? ['--tasks', projectTask] : ['--example', target]),
      '--build-dir', buildRoot, '--trace-dir', join(REPO, '.graph-traces'), '--save-actual'], true);
    testCode = testCode || unitCode || fileCode;
  }
  if (values['prepare-only']) {
    process.exitCode = testCode;
    console.log('\nЗаписи подготовлены.');
    return;
  }
  if (testCode) console.log('\nЕсть непройденные тесты. Их графы и записанные шаги можно изучить в просмотрщике.');
  const server = viewerServer(REPO, join(REPO, '.graph-traces'), projectTask || (task ? examples[0] : undefined));
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
