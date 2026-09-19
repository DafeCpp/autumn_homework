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
test('traversal uses recorded roles, supports forests and rewinds without future edges',()=>{
  const graph={n:5,first:1,edges:[{id:0,from:2,to:1},{id:1,from:1,to:2},{id:2,from:3,to:3},{id:3,from:4,to:3}]};
  const events=[{type:'node',id:1,state:'queued'},{type:'step',message:'root'},
    {type:'node',id:2,state:'queued'},{type:'value',id:2,name:'parent',value:1},
    {type:'edge',id:0,state:'tree'},{type:'edge',id:1,state:'non_tree'},
    {type:'node',id:3,state:'active'},{type:'edge',id:2,state:'back'},
    {type:'edge',id:3,state:'tree'},{type:'edge',id:0,state:'bridge'}];
  const state=M.state(events,events.length-1), layout=M.traversal(graph,state);
  assert.equal(state.roles.get(0),'tree');assert.equal(state.edges.get(0),'bridge');
  assert.equal(state.roles.get(1),'non_tree');assert.equal(state.roles.get(2),'back');
  assert.equal(layout.positions.get(1)[1],layout.positions.get(3)[1]);
  assert.ok(layout.positions.get(2)[1]>layout.positions.get(1)[1]);
  assert.ok(layout.positions.get(4)[1]>layout.positions.get(3)[1]);
  assert.deepEqual(layout.unseen,[5]);
  const earlier=M.state(events,1);
  assert.equal(earlier.roles.size,0);
  assert.deepEqual(M.traversal(graph,earlier).unseen,[2,3,4,5]);
});
