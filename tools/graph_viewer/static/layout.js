/* Детерминированная раскладка: рёбра стягивают вершины, вершины отталкиваются. */
(function (root) {
  // Считает пересечения прямых рёбер без общих концов для сравнения вариантов.
  function crossings(points, edges) {
    const turn = (a,b,c) => (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
    let count = 0;
    for (let i=0;i<edges.length;i++) for (let j=0;j<i;j++) {
      const [a,b]=edges[i], [c,d]=edges[j];
      if (a===c || a===d || b===c || b===d) continue;
      if (turn(points[a],points[b],points[c])*turn(points[a],points[b],points[d])<0 &&
          turn(points[c],points[d],points[a])*turn(points[c],points[d],points[b])<0) count++;
    }
    return count;
  }
  // Несколько начальных положений помогают выйти из неудачной раскладки.
  function componentLayout(n, edges) {
    if (n===1) return [[0,0]];
    let best, bestScore=Infinity;
    const attempts=n<=40 && edges.length<=100 ? 10 : 1;
    for (let attempt=0;attempt<attempts;attempt++) {
      let seed=12345+attempt*997;
      const random=()=>{seed=(Math.imul(seed,1664525)+1013904223)>>>0;return seed/4294967296;};
      const radius=Math.sqrt(n)*60;
      const points=Array.from({length:n},()=>[(random()-.5)*radius,(random()-.5)*radius]);
      const iterations=n<=80 ? 320 : 160;
      for (let step=0;step<iterations;step++) {
        const force=Array.from({length:n},()=>[0,0]);
        for (let i=0;i<n;i++) for(let j=0;j<i;j++) {
          const dx=points[i][0]-points[j][0], dy=points[i][1]-points[j][1];
          const distance=Math.max(1,Math.hypot(dx,dy));
          const strength=6400/(distance*distance);
          force[i][0]+=dx*strength;force[i][1]+=dy*strength;
          force[j][0]-=dx*strength;force[j][1]-=dy*strength;
        }
        for (const [a,b] of edges) {
          const dx=points[b][0]-points[a][0],dy=points[b][1]-points[a][1];
          const strength=Math.hypot(dx,dy)/80;
          force[a][0]+=dx*strength;force[a][1]+=dy*strength;
          force[b][0]-=dx*strength;force[b][1]-=dy*strength;
        }
        const temperature=25*(1-step/iterations)**2+.05;
        for(let i=0;i<n;i++) {
          const length=Math.max(.01,Math.hypot(...force[i]));
          points[i][0]+=force[i][0]/length*Math.min(length,temperature);
          points[i][1]+=force[i][1]/length*Math.min(length,temperature);
        }
      }
      // Пересечения важнее длины; среди равных вариантов предпочитаем короткие рёбра.
      const length=edges.reduce((sum,[a,b])=>sum+Math.hypot(points[a][0]-points[b][0],points[a][1]-points[b][1]),0);
      const score=(attempts>1 ? crossings(points,edges)*1e6 : 0)+length;
      if(score<bestScore){bestScore=score;best=points;}
    }
    return best;
  }
  // Размещает компоненты в отдельных прямоугольниках; петли и кратность не меняют силы.
  function layout(graph) {
    const adjacency=Array.from({length:graph.n},()=>[]), edges=[], seen=new Set();
    for(const e of graph.edges) {
      const a=e.from-graph.first,b=e.to-graph.first;
      const key=`${Math.min(a,b)}:${Math.max(a,b)}`;
      if(a===b || seen.has(key)) continue;
      seen.add(key);edges.push([a,b]);adjacency[a].push(b);adjacency[b].push(a);
    }
    const visited=new Set(), blocks=[];
    for(let start=0;start<graph.n;start++) {
      if(visited.has(start)) continue;
      const vertices=[start];visited.add(start);
      for(let i=0;i<vertices.length;i++) for(const to of adjacency[vertices[i]]) {
        if(!visited.has(to)){visited.add(to);vertices.push(to);}
      }
      const indices=new Map(vertices.map((v,i)=>[v,i]));
      const localEdges=edges.filter(([a])=>indices.has(a)).map(([a,b])=>[indices.get(a),indices.get(b)]);
      const points=componentLayout(vertices.length,localEdges);
      let separation=Infinity;
      for(let i=0;i<points.length;i++) for(let j=0;j<i;j++) separation=Math.min(separation,Math.hypot(points[i][0]-points[j][0],points[i][1]-points[j][1]));
      const scale=Math.max(1,65/Math.max(.01,separation));
      const minX=Math.min(...points.map(p=>p[0])),minY=Math.min(...points.map(p=>p[1]));
      const normalized=points.map(([x,y])=>[(x-minX)*scale+90,(y-minY)*scale+100]);
      blocks.push({vertices,points:normalized,width:Math.max(...normalized.map(p=>p[0]))+90,height:Math.max(...normalized.map(p=>p[1]))+70});
    }
    blocks.sort((a,b)=>b.height-a.height);
    const target=Math.max(900,Math.sqrt(blocks.reduce((sum,b)=>sum+b.width*b.height,0)*1.6));
    const positions=new Map();let x=0,y=0,rowHeight=0,width=900;
    for(const block of blocks) {
      if(x && x+block.width>target){y+=rowHeight+25;x=0;rowHeight=0;}
      block.vertices.forEach((v,i)=>positions.set(v+graph.first,[x+block.points[i][0],y+block.points[i][1]]));
      width=Math.max(width,x+block.width);x+=block.width+25;rowHeight=Math.max(rowHeight,block.height);
    }
    const height=Math.max(570,y+rowHeight);
    // Центрируем маленькие графы в области просмотра.
    const actualWidth=Math.max(0,...[...positions.values()].map(p=>p[0]))+90;
    const actualHeight=y+rowHeight;
    for(const p of positions.values()){p[0]+=(width-actualWidth)/2;p[1]+=(height-actualHeight)/2;}
    return {positions,box:[0,0,width,height]};
  }
  root.GraphLayout={layout,crossings};
})(globalThis);
