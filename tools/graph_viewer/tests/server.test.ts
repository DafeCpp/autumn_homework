import { test } from 'node:test';
import assert from 'node:assert/strict';
import { mkdtemp, mkdir, writeFile, rm, symlink } from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { once } from 'node:events';
import { parseGraph, parseTrace, catalog, caseData, readBounded, viewerServer } from '../server.ts';
import type { AddressInfo } from 'node:net';
const graph = parseGraph('3 2\n1 2\n2 3\n', false, 1);
const header = [
  {type:'header',version:1}, {type:'graph',n:3,first:1,directed:false},
  ...graph.edges.map(e => ({type:'add_edge',...e})),
];
const jsonl = (events: unknown[]) => events.map(e=>JSON.stringify(e)).join('\n')+'\n';

test('graph adapters preserve zero-based IDs, loops, parallel and isolated vertices', () => {
  const g=parseGraph('4 3\n0 0\n0 1\n0 1',true,0);
  assert.equal(g.n,4); assert.equal(g.directed,true);
  assert.deepEqual(g.edges.map(e=>e.id),[0,1,2]);
  assert.throws(()=>parseGraph('2 1\n0 1',false,1),/диапазона/);
  assert.throws(()=>parseGraph('2 2\n1 2',false,1),/Число/);
  assert.throws(()=>parseGraph('501 0',false,1),/небольшой/);
  assert.throws(()=>parseGraph('2 1\n1.0 2',false,1),/целые/);
});
test('partial traces, size limits and mismatched graphs are explicit', () => {
  const events=[...header,{type:'node',id:1,state:'active'},{type:'step',message:'Hello "world"\nПривет'}];
  assert.equal(parseTrace(jsonl([...events,{type:'end'}]),graph).warnings.length,0);
  const partial=parseTrace(jsonl(events)+'{"type":',graph);
  assert.equal(partial.events.length,2); assert.equal(partial.warnings.length,2);
  assert.match(parseTrace(jsonl([...events,{type:'truncated'}]),graph).warnings[0],/размеру/);
  assert.throws(()=>parseTrace(jsonl([...header,{type:'node',id:9,state:'active'}]),graph),/неизвестный/);
  assert.throws(()=>parseTrace(jsonl([{type:'header',version:2}]),graph),/версия/);
  assert.throws(()=>parseTrace(jsonl([...header,{type:'end'},{type:'step',message:'bad'}]),graph),/завершения/);
  assert.throws(()=>parseTrace(jsonl(header.slice(0,-1)),graph),/неполная/);
});
test('all existing supported repository tests parse', async () => {
  const cases=await catalog(); assert.ok(cases.length>=19);
  for (const c of cases) assert.ok((await caseData(c.id)).graph.n>0);
});
test('read-only HTTP API confines files and serves browser assets', async () => {
  const root=await mkdtemp(join(tmpdir(),'graph-server-'));
  const tests=join(root,'task_03','tests');await mkdir(tests,{recursive:true});
  await writeFile(join(tests,'one.in'),'1 0\n');
  const bfs=join(root,'examples','bfs_visualization','tests');
  await mkdir(bfs,{recursive:true});await writeFile(join(bfs,'demo.in'),'1 0\n');
  const server=viewerServer(root,join(root,'.graph-traces'),'task_03');
  try {
    await writeFile(join(root,'secret'),'do not expose');
    await symlink(join(root,'secret'),join(tests,'escape.in'));
    await assert.rejects(readBounded(join(tests,'escape.in'),tests),/пределами/);
    await assert.rejects(readBounded(join(root,'secret'),root,3),/слишком/);
    server.listen(0,'127.0.0.1');await once(server,'listening');
    const base=`http://127.0.0.1:${(server.address() as AddressInfo).port}`;
    assert.equal((await (await fetch(base+'/api/cases')).json())[0].task,'task_03');
    const response=await fetch(base+'/api/case?id=task_03/tests/one.in');
    assert.equal(response.status,200); assert.equal((await response.json()).graph.n,1);
    assert.equal((await fetch(base+'/api/case?id=../../secret')).status,400);
    assert.equal((await fetch(base+'/secret')).status,404);
    assert.equal((await fetch(base+'/api/cases',{method:'POST'})).status,405);
    for (const asset of ['/','/app.js','/model.js','/style.css']) assert.equal((await fetch(base+asset)).status,200);
    assert.match(response.headers.get('content-security-policy')||'',/default-src 'self'/);
  } finally { if(server.listening) await new Promise<void>(done=>server.close(()=>done())); await rm(root,{recursive:true,force:true}); }
});
