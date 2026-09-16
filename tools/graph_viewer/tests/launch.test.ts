import { test } from 'node:test';
import assert from 'node:assert/strict';
import { spawn, spawnSync } from 'node:child_process';
import { mkdtemp, mkdir, readFile, readdir, writeFile, rm } from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { createServer } from 'node:net';
import { once } from 'node:events';
import type { AddressInfo } from 'node:net';
import { REPO } from '../server.ts';
const script = join(REPO, 'visualize');

test('launcher help and argument validation work from outside the repository', () => {
  const help = spawnSync(script, ['--help'], {cwd: tmpdir(), encoding: 'utf8'});
  assert.equal(help.status, 0, help.stderr);
  assert.match(help.stdout, /visualize task_03/);
  for (const args of [['--port', 'wrong'], ['--task', '../other'], ['../other'], ['task_02', 'task_03'], ['task_02', '--task', 'task_03']]) {
    const result = spawnSync(script, args, {cwd: tmpdir(), encoding: 'utf8'});
    assert.equal(result.status, 1);
    assert.doesNotMatch(result.stdout, /1\/3/);
  }
});

test('one command builds BFS, records tests and serves the viewer', {timeout: 60000}, async t => {
  const socket = createServer(); socket.listen(0, '127.0.0.1');
  await once(socket, 'listening');
  const port = (socket.address() as AddressInfo).port;
  await new Promise<void>(done => socket.close(() => done()));
  const child = spawn(script, ['--port', String(port)], {cwd: tmpdir(), stdio: ['ignore', 'pipe', 'pipe']});
  let output = '', errors = '';
  child.stderr.on('data', chunk => { errors += chunk; });
  t.after(async () => {
    if (child.exitCode === null && child.signalCode === null) {
      child.kill('SIGTERM'); await once(child, 'exit');
    }
  });
  await new Promise<void>((done, reject) => {
    const deadline = setTimeout(() => reject(Error('Launcher timed out: '+output+errors)), 50000);
    child.once('error', error => { clearTimeout(deadline); reject(error); });
    child.once('exit', code => {clearTimeout(deadline); reject(Error(`Launcher exited ${code}: ${output}${errors}`));});
    child.stdout.on('data', chunk => {
      output += chunk;
      if (output.includes('Просмотрщик готов:')) { clearTimeout(deadline); done(); }
    });
  });
  assert.match(output, /12 tests from 1 test suite/);
  // Число примеров меняется при добавлении графов: сверяем запуск с файлами.
  const cases = (await readdir(join(REPO, 'examples/bfs_visualization/tests')))
    .filter(name => name.endsWith('.in'));
  assert.ok(cases.length > 0);
  assert.equal(Number(output.match(/^Cases run: (\d+)$/m)?.[1]), cases.length);
  for (const file of cases) {
    assert.ok(output.split('\n').some(line => line.startsWith(`${file.slice(0, -3)}: OK (`)),
      `Missing successful result for ${file}`);
  }
  const response = await fetch(`http://127.0.0.1:${port}/api/case?id=examples/bfs_visualization/tests/chain.in`);
  assert.equal(response.status, 200);
  const body = await response.json();
  assert.ok(body.events.length > 0);
  assert.deepEqual(body.warnings, []);
  child.kill('SIGTERM');
  const [code] = await once(child, 'exit');
  assert.equal(code, 0);
});


test('project CMake ignores generated directories named like tasks', async () => {
  const root = await mkdtemp(join(tmpdir(), 'graph-cmake-'));
  try {
    await writeFile(join(root, 'CMakeLists.txt'), await readFile(join(REPO, 'CMakeLists.txt')));
    for (const folder of ['lib', 'sandbox', 'additional_tasks', 'examples/bfs_visualization', 'examples/kosaraju_visualization', 'examples/tarjan_visualization', 'task_03']) {
      await mkdir(join(root, folder), {recursive: true});
      await writeFile(join(root, folder, 'CMakeLists.txt'), folder === 'task_03' ? 'add_custom_target(task_03)\n' : '');
    }
    await mkdir(join(root, '.graph-traces/task_03'), {recursive: true});
    await mkdir(join(root, '.graph-build/project/task_03'), {recursive: true});
    const result = spawnSync('cmake', ['-S', root, '-B', join(root, 'build')], {encoding: 'utf8'});
    assert.equal(result.status, 0, result.stdout + result.stderr);
    const build = spawnSync('cmake', ['--build', join(root, 'build'), '--target', 'task_03'], {encoding: 'utf8'});
    assert.equal(build.status, 0, build.stdout + build.stderr);
  } finally { await rm(root, {recursive: true, force: true}); }
});
