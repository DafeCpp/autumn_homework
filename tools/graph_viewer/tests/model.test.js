import {test} from 'node:test';
import assert from 'node:assert/strict';
import '../static/model.js';
const M=globalThis.GraphModel;
test('backward playback rebuilds exact state without future values',()=>{
  const events=[{type:'node',id:1,state:'queued'},{type:'step',message:'one'},{type:'node',id:1,state:'done'},{type:'value',id:2,name:'distance',value:1},{type:'step',message:'two'}];
  assert.deepEqual(M.steps(events),[-1,1,4]);
  assert.equal(M.state(events,4).nodes.get(1),'done');
  assert.equal(M.state(events,1).nodes.get(1),'queued');
  assert.equal(M.state(events,1).values.has(2),false);
  assert.equal(M.state(events,-1).nodes.size,0);
});
test('experiments handle disconnected graphs, articulation points and parallel edges',()=>{
  const graph={n:5,first:1,edges:[{id:0,from:1,to:2},{id:1,from:2,to:3},{id:2,from:4,to:5},{id:3,from:4,to:5}]};
  assert.equal(M.components(graph).count,2);
  assert.equal(M.components(graph,new Set([2])).count,3);
  assert.equal(M.components(graph,new Set(),new Set([2])).count,2);
  assert.equal(M.components(graph,new Set(),new Set([0])).count,3);
});
test('task_03 highlights only validated answer elements',()=>{
  const graph={n:3,first:1,edges:[{id:0,from:1,to:2},{id:1,from:2,to:3}]};
  const answer=M.answer('task_03','1\n2\n2\n1 2; 2 3\n',graph);
  assert.deepEqual([...answer.nodes],[2]);assert.deepEqual([...answer.edges],[0,1]);
  assert.throws(()=>M.answer('task_03','0\n-\n1\n1 3',graph),/Неизвестное ребро/);
});
