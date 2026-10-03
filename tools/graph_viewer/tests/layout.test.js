import {test} from 'node:test';
import assert from 'node:assert/strict';
import '../static/layout.js';
const L=globalThis.GraphLayout;
const graph=(n,pairs,first=1)=>({n,first,edges:pairs.map(([from,to],id)=>({id,from,to}))});
test('automatic layout untangles a grid and is deterministic',()=>{
  const pairs=[];
  for(let r=0;r<4;r++) for(let c=0;c<4;c++) {
    const v=r*4+c+1;
    if(c<3)pairs.push([v,v+1]);if(r<3)pairs.push([v,v+4]);
  }
  const g=graph(16,pairs),result=L.layout(g);
  assert.deepEqual(L.layout(g),result);
  const points=Array.from({length:16},(_,i)=>result.positions.get(i+1));
  assert.equal(L.crossings(points,pairs.map(([a,b])=>[a-1,b-1])),0);
  for(let i=0;i<16;i++) for(let j=0;j<i;j++) assert.ok(Math.hypot(points[i][0]-points[j][0],points[i][1]-points[j][1])>=64.9);
});
test('loops, parallel edges, directed edges and isolated zero-based vertices have valid positions',()=>{
  const base=graph(6,[[0,1],[1,2],[3,4]],0);
  const result=L.layout(base);
  assert.deepEqual(L.layout({...base,directed:true,edges:[...base.edges,{id:3,from:1,to:0},{id:4,from:2,to:2}]}),result);
  assert.equal(result.positions.size,6);
  for(const [x,y] of result.positions.values()) {
    assert.ok(Number.isFinite(x)&&Number.isFinite(y));
    assert.ok(x>=20&&x<=result.box[2]-20&&y>=20&&y<=result.box[3]-20);
  }
  const bounds=ids=>ids.map(id=>result.positions.get(id));
  for(const a of bounds([0,1,2])) for(const b of bounds([3,4,5])) assert.ok(Math.hypot(a[0]-b[0],a[1]-b[1])>65);
  assert.equal(L.layout(graph(1,[])).positions.size,1);
});
test('maximum supported vertex count produces finite positions',()=>{
  const g=graph(500,Array.from({length:499},(_,i)=>[i+1,i+2]));
  const result=L.layout(g);
  assert.equal(result.positions.size,500);
  assert.ok([...result.positions.values()].flat().every(Number.isFinite));
});
