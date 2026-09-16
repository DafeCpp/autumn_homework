'use strict';
const $ = id => document.getElementById(id);
const M = GraphModel;
let data = null, steps = [-1], index = 0, timer = null;
let removedNodes = new Set(), removedEdges = new Set(), signature = '', loading = false;
let treeKey = '';
// У каждой области свои координаты и масштаб; шаг записи общий.
const views = ['graph', 'tree'].map(id => ({svg: $(id), positions: new Map(), box: [0,0,900,570]}));
function element(tag, attrs = {}, text = null) {
  const el = document.createElementNS('http://www.w3.org/2000/svg', tag);
  for (const [key, value] of Object.entries(attrs)) el.setAttribute(key, value);
  if (text !== null) el.textContent = text;
  return el;
}
function stop() { clearTimeout(timer); timer = null; $('play').textContent = '▶ Воспроизвести'; }
function showError(error) { $('notice').textContent = error.message; }
async function get(url) {
  const r = await fetch(url); const body = await r.json();
  if (!r.ok) throw Error(body.error || 'Не удалось загрузить данные');
  return body;
}
function resetPositions() {
  const layout = GraphLayout.layout(data.graph);
  views[0].positions = layout.positions; views[0].box = layout.box; treeKey = '';
}
async function load(refresh = false) {
  if (loading) return;
  const requested = $('case').value;
  if (!requested) return;
  loading = true;
  try {
    const next = await get('api/case?id=' + encodeURIComponent(requested));
    if ($('case').value !== requested) return;
    const encoded = JSON.stringify(next);
    if (refresh && encoded === signature) { $('notice').textContent = next.warnings.join('\n'); return; }
    const sameGraph = data && data.id === next.id && JSON.stringify(data.graph) === JSON.stringify(next.graph);
    stop(); data = next; signature = encoded; steps = M.steps(data.events);
    index = refresh && sameGraph ? Math.min(index, steps.length - 1) : 0;
    if (!sameGraph) { resetPositions(); removedNodes.clear(); removedEdges.clear(); $('answer').value = ''; }
    $('description').textContent = data.label;
    $('input').textContent = data.input;
    $('expected').textContent = data.expected || 'Нет файла эталона';
    $('actual').textContent = data.actual || 'Нет сохранённого несовпадения';
    $('graph-title').textContent = `${data.name} · ${data.graph.n} вершин · ${data.graph.edges.length} рёбер`;
    $('notice').textContent = data.warnings.join('\n');
    $('answer').disabled = data.task !== 'task_03';
    $('experiment').disabled = data.graph.directed;
    if (data.graph.directed) $('experiment').checked = false;
    render();
  } catch (error) {
    if (!refresh) { data = null; views.forEach(view => view.svg.replaceChildren()); $('values').replaceChildren(); stop(); }
    showError(error);
  } finally { loading = false; if ($('case').value !== requested) load(); }
}
function edgePath(e, positions, curved = false) {
  const [x1, y1] = positions.get(e.from), [x2, y2] = positions.get(e.to);
  if (e.from === e.to) {
    const loop = data.graph.edges.filter(x => x.from === e.from && x.to === e.to).findIndex(x => x.id === e.id);
    return `M ${x1-12} ${y1-15} C ${x1-65-loop*14} ${y1-85-loop*14}, ${x1+65+loop*14} ${y1-85-loop*14}, ${x1+12} ${y1-15}`;
  }
  const peers = data.graph.edges.filter(x => Math.min(x.from,x.to) === Math.min(e.from,e.to) && Math.max(x.from,x.to) === Math.max(e.from,e.to));
  const offset = (peers.findIndex(x => x.id === e.id) - (peers.length-1)/2) * 35 * (e.from < e.to ? 1 : -1) + (curved ? 100 : 0);
  const length = Math.hypot(x2-x1, y2-y1) || 1;
  const cx = (x1+x2)/2 - (y2-y1)/length*offset, cy = (y1+y2)/2 + (x2-x1)/length*offset;
  const startLength = Math.hypot(cx-x1,cy-y1) || 1, endLength = Math.hypot(x2-cx,y2-cy) || 1;
  return `M ${x1+(cx-x1)/startLength*21} ${y1+(cy-y1)/startLength*21} Q ${cx} ${cy} ${x2-(x2-cx)/endLength*24} ${y2-(y2-cy)/endLength*24}`;
}
function render() {
  if (!data) return;
  const state = M.state(data.events, steps[index]);
  const mode = $('view-mode').value;
  const scc = ['kosaraju_visualization', 'tarjan_visualization'].includes(data.task);
  const transposed = state.values.get(data.graph.first)?.get('transpose') === 1;
  document.querySelector('#graph-panel .panel-title').textContent = transposed ? 'Обратный граф Gᵀ' : 'Исходный граф';
  $('scc-state').hidden = !scc;
  $('queued-label').textContent = data.task === 'tarjan_visualization' ? 'В стеке КСС после выхода из DFS' : 'В очереди';
  $('active-label').textContent = scc ? 'В стеке DFS' : 'Текущая';
  if (scc) {
    const entries = [...state.values];
    const stack = entries.filter(([, v]) => v.get('on_stack') === 1)
      .sort((a,b) => a[1].get('stack_position') - b[1].get('stack_position')).map(([v]) => v);
    const finish = entries.filter(([, v]) => v.has('finish'))
      .sort((a,b) => a[1].get('finish') - b[1].get('finish')).map(([v]) => v);
    const groups = new Map();
    for (const [v, vars] of entries) {
      const c = vars.get('component');
      if (c) { if (!groups.has(c)) groups.set(c, []); groups.get(c).push(v); }
    }
    const details = data.task === 'tarjan_visualization'
      ? `Стек КСС (дно → вершина): ${stack.join(' → ') || 'пуст'}. Жёлтые вершины завершили DFS, но ещё в стеке.`
      : `Порядок выхода: ${finish.join(' → ') || 'пуст'}. Второй проход: ${[...finish].reverse().join(' → ') || '—'}.`;
    $('scc-state').textContent = details + ' КСС: ' + ([...groups].map(([c, vs]) =>
      `${c} = {${vs.sort((a,b)=>a-b).join(', ')}}`).join('; ') || 'пока не выделены');
  }
  const key = signature + ':' + index;
  if (treeKey !== key) {
    const layout = M.traversal(data.graph, state);
    views[1].positions = layout.positions; views[1].box = layout.box; treeKey = key;
  }
  $('graph-panel').hidden = mode === 'tree';
  $('tree-panel').hidden = mode === 'graph';
  $('canvases').classList.toggle('split', mode === 'both');
  $('tree-note').hidden = mode === 'graph';
  $('tree-note').textContent = 'Показаны только записанные рёбра обхода. Непосещённые вершины — в нижнем ряду. В BFS пунктир означает нетревесные рёбра; обратные рёбра DFS ведут к предку.';
  if (data.task === 'tarjan_visualization') $('tree-note').textContent = 'Сплошные рёбра — дерево DFS, пунктир — нетревесные рёбра. Ребро в стек КСС может вести к вершине, уже вышедшей из DFS.';
  let answer = {nodes:new Set(), edges:new Set()};
  try { answer = M.answer(data.task, data[$('answer').value] || '', data.graph); } catch(error) { showError(error); }
  views.forEach((view, i) => drawGraph(view, i === 1, state, answer));
  $('values').replaceChildren();
  const labels = {queued:data.task === 'tarjan_visualization' ? 'В стеке КСС' : 'В очереди',active:scc ? 'В стеке DFS' : 'Текущая',done:'Обработана',cut:'Критическая'};
  for (const v of views[0].positions.keys()) {
    const row = document.createElement('tr');
    for (const value of [v, labels[state.nodes.get(v)] || 'Не посещена', [...(state.values.get(v)||[])].map(([k,val])=>`${k} = ${val}`).join(', ') || '—']) {
      const cell = document.createElement('td');cell.textContent = value;row.append(cell);
    }
    $('values').append(row);
  }
  const hasSteps = steps.length > 1;
  $('counter').textContent = hasSteps ? `${index} / ${steps.length-1}` : 'Запись не найдена';
  $('step-label').textContent = index === 0 ? (hasSteps ? 'Исходное состояние графа' : 'Граф доступен без записи. Запустите решение с --trace-dir, чтобы увидеть шаги.') : data.events[steps[index]].message;
  $('timeline').max = steps.length-1;$('timeline').value = index;
  for (const id of ['first','previous']) $(id).disabled = index === 0;
  $('next').disabled = index === steps.length-1;
  $('play').disabled = !hasSteps; $('timeline').disabled = !hasSteps;
  const before = M.components(data.graph).count;
  $('components').textContent = data.graph.directed ? 'Отключения доступны для неориентированных графов' : `Компонент: ${before} → ${M.components(data.graph,removedNodes,removedEdges).count}`;
}
function drawGraph(view, tree, state, answer) {
  const {svg, positions, box} = view;
  svg.replaceChildren(); svg.setAttribute('viewBox',box.join(' '));
  const defs = element('defs'), marker = element('marker',{id:`arrow-${svg.id}`,viewBox:'0 0 10 10',refX:9,refY:5,markerWidth:6,markerHeight:6,orient:'auto-start-reverse'});
  marker.append(element('path',{d:'M 0 0 L 10 5 L 0 10 z',fill:'#6b897b'}));defs.append(marker);svg.append(defs);
  for (const e of data.graph.edges) {
    const role = state.roles.get(e.id) || '';
    if (tree && !role) continue;
    const hidden = removedEdges.has(e.id) || removedNodes.has(e.from) || removedNodes.has(e.to);
    const d = edgePath(e, positions, tree && ['back', 'non_tree'].includes(role));
    const path = element('path',{d,class:`edge ${role} ${state.edges.get(e.id)||''} ${answer.edges.has(e.id)?'answer':''} ${hidden?'removed':''}`});
    if (data.graph.directed) path.setAttribute(state.values.get(data.graph.first)?.get('transpose') === 1 ? 'marker-start' : 'marker-end',`url(#arrow-${svg.id})`);
    const hit = element('path',{d,class:'edge-hit','data-edge':e.id});
    hit.append(element('title',{},`Ребро ${e.id}: ${e.from} ${data.graph.directed ? (state.values.get(data.graph.first)?.get('transpose') === 1 ? '←' : '→') : '—'} ${e.to}`));svg.append(path,hit);
  }
  for (const [v, [x,y]] of positions) {
    const g = element('g',{transform:`translate(${x} ${y})`,class:`node ${state.nodes.get(v)||''} ${answer.nodes.has(v)?'answer':''} ${removedNodes.has(v)?'removed':''}`,'data-node':v});
    const circle = element('circle',{r:20});
    const tarjan = data.task === 'tarjan_visualization';
    const values = state.values.get(v);
    const component = state.values.get(v)?.get('component');
    if (component) {
      circle.style.fill = `hsl(${(component * 137.508) % 360} 65% 85%)`;
      g.append(element('text',{y:tarjan ? 63 : 33,class:'distance'},`КСС ${component}`));
    }
    g.append(circle,element('text',{},v));
    if (tarjan) {
      // Read the selected frame, so rewinding also restores earlier lowlink values.
      g.append(element('text',{y:33,class:'scc-value'},`tin = ${values?.get('index') ?? '—'}`),
        element('text',{y:48,class:'scc-value'},`low = ${values?.get('lowlink') ?? '—'}`));
    }
    const distance = state.values.get(v)?.get('distance');
    if (distance !== undefined) g.append(element('text',{y:33,class:'distance'},`d = ${distance}`));
    svg.append(g);
  }
}
function setStep(value) { stop(); index = Math.max(0,Math.min(steps.length-1,value));render(); }
$('first').onclick = () => setStep(0);
$('previous').onclick = () => setStep(index-1);
$('next').onclick = () => setStep(index+1);
$('timeline').oninput = e => setStep(Number(e.target.value));
$('play').onclick = () => {
  if (timer) return stop();
  if (index === steps.length-1) index = 0;
  $('play').textContent = 'Ⅱ Пауза';
  const tick = () => { if (index >= steps.length-1) return stop();index++;render();timer = setTimeout(tick,Number($('speed').value)); };
  timer = setTimeout(tick,Number($('speed').value));
};
$('case').onchange = () => { stop(); load(); };
$('answer').onchange = render;
$('view-mode').onchange = render;
$('restore').onclick = () => { removedNodes.clear();removedEdges.clear();render(); };
$('experiment').onchange = () => { if (!$('experiment').checked) { removedNodes.clear();removedEdges.clear();render(); } };
function zoom(view, factor) {
  const box = view.box;
  view.box = [box[0]+box[2]*(1-factor)/2,box[1]+box[3]*(1-factor)/2,box[2]*factor,box[3]*factor];
  view.svg.setAttribute('viewBox',view.box.join(' '));
}
function zoomVisible(factor) { views.filter(view => !view.svg.parentElement.hidden).forEach(view => zoom(view, factor)); }
$('zoom-in').onclick = () => zoomVisible(.8); $('zoom-out').onclick = () => zoomVisible(1.25);
$('fit').onclick = () => { if(data){resetPositions();render();} };
for (const view of views) {
  const svg = view.svg;
  let drag = null, dragged = false, startedOnBackground = false;
  const point = e => new DOMPoint(e.clientX,e.clientY).matrixTransform(svg.getScreenCTM().inverse());
  svg.addEventListener('wheel', e => {e.preventDefault();zoom(view,e.deltaY>0?1.1:.9);},{passive:false});
  svg.onpointerdown = e => {
    if (!data) return;
    // Захват указателя может перенаправить dblclick на SVG даже после щелчка по вершине.
    startedOnBackground = e.target === svg;
    const node = e.target.closest('[data-node]'), edge = e.target.closest('[data-edge]');
    drag = {node:node?Number(node.dataset.node):null,edge:edge?Number(edge.dataset.edge):null,p:point(e),x:e.clientX,y:e.clientY};
    dragged=false; svg.setPointerCapture(e.pointerId);
  };
  svg.onpointermove = e => {
    if (!drag || (Math.hypot(e.clientX-drag.x,e.clientY-drag.y)<4 && !dragged)) return;
    dragged=true; const p=point(e);
    if (drag.node !== null) view.positions.set(drag.node,[p.x,p.y]);
    else {view.box[0]-=p.x-drag.p.x;view.box[1]-=p.y-drag.p.y;}
    render();
  };
  svg.onpointerup = e => {
    const target = drag; drag = null; svg.releasePointerCapture(e.pointerId);
    if (!target || !data || dragged || !$('experiment').checked || data.graph.directed) return;
    const collection = target.node !== null ? removedNodes : removedEdges;
    const id = target.node !== null ? target.node : target.edge;
    if (id === null) return;
    collection.has(id) ? collection.delete(id) : collection.add(id); render();
  };
  svg.onpointercancel = () => {drag=null;};
  // Двойной щелчок по фону открывает полный экран, не затрагивая элементы графа.
  svg.ondblclick = e => {
    if (e.target !== svg || !startedOnBackground || dragged || document.fullscreenElement === workspace || workspace.classList.contains('expanded')) return;
    toggleFullscreen();
  };
}
const workspace = $('viewer');
function fullscreenState() {
  const expanded = document.fullscreenElement === workspace || workspace.classList.contains('expanded');
  $('fullscreen').textContent = expanded ? 'Свернуть' : 'На весь экран';
  $('fullscreen').setAttribute('aria-pressed', String(expanded));
}
async function toggleFullscreen() {
  if (document.fullscreenElement === workspace) await document.exitFullscreen();
  else if (workspace.classList.contains('expanded')) workspace.classList.remove('expanded');
  else {
    try {
      if (!workspace.requestFullscreen) throw Error('Fullscreen unavailable');
      await workspace.requestFullscreen();
    } catch {
      // Если браузер запрещает Fullscreen API, разворачиваем область внутри вкладки.
      workspace.classList.add('expanded');
    }
  }
  fullscreenState();
}
$('fullscreen').onclick = toggleFullscreen;
document.addEventListener('fullscreenchange', fullscreenState);
document.addEventListener('keydown', e => {
  if (e.key === 'Escape') { workspace.classList.remove('expanded'); fullscreenState(); }
});
async function init() {
  try {
    const cases=await get('api/cases');$('case').replaceChildren();
    for(const c of cases){const option=document.createElement('option');option.value=c.id;option.textContent=`${c.task} / ${c.name}`;$('case').append(option);}
    if(!cases.length) throw Error('Не найдены файловые тесты');
    await load();
  } catch(error){showError(error);}
}
init();setInterval(()=>load(true),3000);
