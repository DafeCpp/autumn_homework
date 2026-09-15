import { test } from 'node:test';
import assert from 'node:assert/strict';
import { spawn, spawnSync } from 'node:child_process';
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
  assert.match(output, /chain: OK/);
  assert.match(output, /Cases run: 6/);
  const response = await fetch(`http://127.0.0.1:${port}/api/case?id=examples/bfs_visualization/tests/chain.in`);
  assert.equal(response.status, 200);
  const body = await response.json();
  assert.ok(body.events.length > 0);
  assert.deepEqual(body.warnings, []);
  child.kill('SIGTERM');
  const [code] = await once(child, 'exit');
  assert.equal(code, 0);
});
