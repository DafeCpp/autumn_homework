import { test } from 'node:test';
import assert from 'node:assert/strict';
import { spawnSync } from 'node:child_process';
import { join } from 'node:path';
import { caseData, REPO } from '../server.ts';
import '../static/model.js';

// Exercise the real C++ → recorder → runner → server → playback pipeline.
test('SCC demos build, validate answers and restore both passes and stacks', {timeout: 60000}, async () => {
  const result = spawnSync(join(REPO, 'visualize'), ['scc', '--prepare-only'],
    {encoding: 'utf8', timeout: 55000});
  assert.equal(result.status, 0, result.stdout + result.stderr);
  assert.equal((result.stdout.match(/Cases run: 6/g) || []).length, 2);
  // The browser model is also consumed directly by existing JS tests.
  const M = (globalThis as any).GraphModel;
  for (const algorithm of ['kosaraju', 'tarjan']) {
    for (const name of ['three_components', 'disconnected', 'dag', 'single', 'loops_parallel', 'stack_cross_edge']) {
      const data = await caseData(`examples/${algorithm}_visualization/tests/${name}.in`);
      assert.deepEqual(data.warnings, []);
      assert.ok(data.events.length > 0);
      const final = M.state(data.events, data.events.length - 1);
      const expected = data.expected.trim().split(/\s+/).map(Number);
      for (let u = 1; u <= data.graph.n; u++) {
        assert.ok(final.values.get(u).get('component') > 0);
        for (let v = 1; v <= data.graph.n; v++) {
          assert.equal(final.values.get(u).get('component') === final.values.get(v).get('component'),
            expected[u - 1] === expected[v - 1]);
        }
      }
      if (algorithm === 'kosaraju') {
        const transition = data.events.findIndex(e => e.type === 'value' && e.name === 'transpose');
        const before = M.state(data.events, transition - 1);
        const after = M.state(data.events, transition);
        assert.equal(after.roles.size, 0);
        assert.equal(after.nodes.size, 0);
        assert.equal(M.traversal(data.graph, after).unseen.length, data.graph.n);
        const transitionStep = data.events.findIndex((e, i) => i > transition && e.type === 'step');
        assert.equal(M.traversal(data.graph, M.state(data.events, transitionStep)).unseen.length, data.graph.n);
        assert.equal(after.values.get(1).get('transpose'), 1);
        for (const vars of after.values.values()) {
          assert.equal(vars.has('parent'), false);
          assert.ok(vars.has('finish'));
        }
        assert.equal(before.values.get(1).has('transpose'), false);
        assert.deepEqual(M.state(data.events, transition - 1), before);
      } else {
        for (const vars of final.values.values()) assert.equal(vars.get('on_stack'), 0);
        for (const step of M.steps(data.events).slice(1)) {
          const state = M.state(data.events, step);
          const positions = [...state.values.values()].filter(v => v.get('on_stack') === 1)
            .map(v => v.get('stack_position')).sort((a,b) => a-b);
          assert.deepEqual(positions, positions.map((_, i) => i + 1));
        }
      }
    }
  }
});
