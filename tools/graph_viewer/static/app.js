'use strict';
const $ = id => document.getElementById(id);
const M = GraphModel;
let data = null, steps = [-1], index = 0, positions = new Map(), timer = null;
let removedNodes = new Set(), removedEdges = new Set(), signature = '', loading = false;
let box = [0, 0, 900, 570], drag = null, dragged = false;
const svg = $('graph');
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
  positions = new Map();
  for (let i = 0; i < data.graph.n; i++) {
    const angle = -Math.PI / 2 + 2 * Math.PI * i / data.graph.n;
    positions.set(i + data.graph.first, data.graph.n === 1 ? [450, 275] : [450 + 225 * Math.cos(angle), 275 + 215 * Math.sin(angle)]);
  }
  box = [0, 0, 900, 570];
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
    if (!refresh) { data = null; svg.replaceChildren(); $('values').replaceChildren(); stop(); }
    showError(error);
  } finally { loading = false; if ($('case').value !== requested) load(); }
}
function edgePath(e) {
  const [x1, y1] = positions.get(e.from), [x2, y2] = positions.get(e.to);
  if (e.from === e.to) {
    const loop = data.graph.edges.filter(x => x.from === e.from && x.to === e.to).findIndex(x => x.id === e.id);
    return `M ${x1-12} ${y1-15} C ${x1-65-loop*14} ${y1-85-loop*14}, ${x1+65+loop*14} ${y1-85-loop*14}, ${x1+12} ${y1-15}`;
  }
  const peers = data.graph.edges.filter(x => Math.min(x.from,x.to) === Math.min(e.from,e.to) && Math.max(x.from,x.to) === Math.max(e.from,e.to));
  const offset = (peers.findIndex(x => x.id === e.id) - (peers.length-1)/2) * 35 * (e.from < e.to ? 1 : -1);
  const length = Math.hypot(x2-x1, y2-y1) || 1;
  const cx = (x1+x2)/2 - (y2-y1)/length*offset, cy = (y1+y2)/2 + (x2-x1)/length*offset;
  const startLength = Math.hypot(cx-x1,cy-y1) || 1, endLength = Math.hypot(x2-cx,y2-cy) || 1;
  return `M ${x1+(cx-x1)/startLength*21} ${y1+(cy-y1)/startLength*21} Q ${cx} ${cy} ${x2-(x2-cx)/endLength*24} ${y2-(y2-cy)/endLength*24}`;
}
function render() {
  if (!data) return;
  const state = M.state(data.events, steps[index]);
  let answer = {nodes:new Set(), edges:new Set()};
  try { answer = M.answer(data.task, data[$('answer').value] || '', data.graph); } catch(error) { showError(error); }
  svg.replaceChildren(); svg.setAttribute('viewBox',box.join(' '));
  const defs = element('defs'), marker = element('marker',{id:'arrow',viewBox:'0 0 10 10',refX:9,refY:5,markerWidth:6,markerHeight:6,orient:'auto-start-reverse'});
  marker.append(element('path',{d:'M 0 0 L 10 5 L 0 10 z',fill:'#6b897b'}));defs.append(marker);svg.append(defs);
  for (const e of data.graph.edges) {
    const hidden = removedEdges.has(e.id) || removedNodes.has(e.from) || removedNodes.has(e.to);
    const d = edgePath(e);
    const path = element('path',{d,class:`edge ${state.edges.get(e.id)||''} ${answer.edges.has(e.id)?'answer':''} ${hidden?'removed':''}`});
    if (data.graph.directed) path.setAttribute('marker-end','url(#arrow)');
    const hit = element('path',{d,class:'edge-hit','data-edge':e.id});
    hit.append(element('title',{},`Ребро ${e.id}: ${e.from} — ${e.to}`));svg.append(path,hit);
  }
  for (const [v, [x,y]] of positions) {
    const g = element('g',{transform:`translate(${x} ${y})`,class:`node ${state.nodes.get(v)||''} ${answer.nodes.has(v)?'answer':''} ${removedNodes.has(v)?'removed':''}`,'data-node':v});
    g.append(element('circle',{r:20}),element('text',{},v));
    const distance = state.values.get(v)?.get('distance');
    if (distance !== undefined) g.append(element('text',{y:33,class:'distance'},`d = ${distance}`));
    svg.append(g);
  }
  $('values').replaceChildren();
  const labels = {queued:'В очереди',active:'Текущая',done:'Обработана',cut:'Критическая'};
  for (const v of positions.keys()) {
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
$('restore').onclick = () => { removedNodes.clear();removedEdges.clear();render(); };
$('experiment').onchange = () => { if (!$('experiment').checked) { removedNodes.clear();removedEdges.clear();render(); } };
function zoom(factor) { box = [box[0]+box[2]*(1-factor)/2,box[1]+box[3]*(1-factor)/2,box[2]*factor,box[3]*factor];svg.setAttribute('viewBox',box.join(' ')); }
$('zoom-in').onclick = () => zoom(.8);$('zoom-out').onclick = () => zoom(1.25);
$('fit').onclick = () => { if(data){resetPositions();render();} };
svg.addEventListener('wheel', e => {e.preventDefault();zoom(e.deltaY>0?1.1:.9);},{passive:false});
function point(e) { const p = new DOMPoint(e.clientX,e.clientY);return p.matrixTransform(svg.getScreenCTM().inverse()); }
svg.onpointerdown = e => {
  if (!data) return;
  const node = e.target.closest('[data-node]'), edge = e.target.closest('[data-edge]');
  drag = {node:node?Number(node.dataset.node):null,edge:edge?Number(edge.dataset.edge):null,p:point(e),x:e.clientX,y:e.clientY};dragged=false;
  svg.setPointerCapture(e.pointerId);
};
svg.onpointermove = e => {
  if (!drag) return;
  if (Math.hypot(e.clientX-drag.x,e.clientY-drag.y)<4 && !dragged) return;
  dragged=true;const p=point(e);
  if (drag.node !== null) positions.set(drag.node,[p.x,p.y]);
  else {box[0]-=p.x-drag.p.x;box[1]-=p.y-drag.p.y;}
  render();
};
svg.onpointerup = e => {
  const target = drag; drag = null;
  svg.releasePointerCapture(e.pointerId);
  if (!target || !data || dragged || !$('experiment').checked || data.graph.directed) return;
  const collection = target.node !== null ? removedNodes : removedEdges;
  const id = target.node !== null ? target.node : target.edge;
  if (id === null) return;
  collection.has(id) ? collection.delete(id) : collection.add(id);
  render();
};
svg.onpointercancel = () => {drag=null;};
async function init() {
  try {
    const cases=await get('api/cases');$('case').replaceChildren();
    for(const c of cases){const option=document.createElement('option');option.value=c.id;option.textContent=`${c.task} / ${c.name}`;$('case').append(option);}
    if(!cases.length) throw Error('Не найдены файловые тесты');
    await load();
  } catch(error){showError(error);}
}
init();setInterval(()=>load(true),3000);
