'use strict';
const $ = id => document.getElementById(id);
const M = GraphModel;
let data = null, steps = [-1], index = 0, timer = null;
let signature = '', loading = false;
let treeKey = '';
// У каждой области свои координаты и масштаб; шаг записи общий.
const views = ['graph', 'tree'].map(id => ({svg: $(id), positions: new Map(), box: [0,0,900,570], baseWidth: 900}));
function element(tag, attrs = {}, text = null) {
  const el = document.createElementNS('http://www.w3.org/2000/svg', tag);
  for (const [key, value] of Object.entries(attrs)) el.setAttribute(key, value);
  if (text !== null) el.textContent = text;
  return el;
}
function playbackButton(playing) {
  const label = playing ? 'Пауза' : 'Воспроизвести';
  $('play').setAttribute('aria-label', label); $('play').title = label;
  $('play').querySelector('path').setAttribute('d', playing ? 'M9 5v14 M15 5v14' : 'm8 5 11 7-11 7z');
}
function stop() { clearTimeout(timer); timer = null; playbackButton(false); }
function updateCaseStatus() {
  const test = catalogCases.find(c => c.id === $('case').value);
  $('case-verdict').textContent = test?.verdict || '';
  $('case-verdict').dataset.status = test?.status || 'not_run';
  $('case-verdict').dataset.stale = String(test?.verdict === 'Нужно перезапустить');
  const mismatch = test?.status === 'fail' && data?.id === test.id && data.actual !== null;
  $('actual').classList.toggle('wrong-output', mismatch);
  $('actual-status').hidden = !mismatch;
  $('actual-status').classList.toggle('wrong', mismatch);
  $('actual-status').textContent = mismatch ? 'Не совпадает' : '';
}
function updateZoom() {
  const visible = views.filter(view => !view.svg.parentElement.hidden);
  $('zoom-level').textContent = visible.map(view => Math.round(100 * view.baseWidth / view.box[2]) + '%').filter((x,i,a) => a.indexOf(x) === i).join(' / ');
}
function setInspector(open) {
  $('inspector-content').hidden = !open;
  $('viewer').classList.toggle('inspector-closed', !open);
  $('inspector-toggle').setAttribute('aria-expanded', String(open));
  $('inspector-toggle').textContent = open ? 'Свернуть' : 'Развернуть';
}
function clearSelection(message = 'Выберите тест') {
  stop(); data = null; signature = ''; steps = [-1]; index = 0;
  views.forEach(view => view.svg.replaceChildren());
  $('values').replaceChildren();
  for (const id of ['input','expected','actual','case-verdict','description']) $(id).textContent = '';
  $('graph-title').textContent = message;
  $('step-kicker').textContent = 'Шаги решения'; $('step-label').textContent = message;
  $('counter').textContent = '— / —'; $('timeline').value = $('timeline').max = 0;
  for (const id of ['first','previous','play','next','timeline','speed']) $(id).disabled = true;
  $('answer-tools').hidden = $('tree-note').hidden = $('actual-status').hidden = true;
  $('actual').classList.remove('wrong-output');
}
function showError(error) { $('notice').textContent = error.message; }
async function get(url) {
  const r = await fetch(url); const body = await r.json();
  if (!r.ok) throw Error(body.error || 'Не удалось загрузить данные');
  return body;
}
function fittedBox(positions, padding = 70) {
  const points = [...positions.values()];
  if (!points.length) return [0,0,900,570];
  const xs = points.map(p => p[0]), ys = points.map(p => p[1]);
  const left = Math.min(...xs), right = Math.max(...xs), top = Math.min(...ys), bottom = Math.max(...ys);
  const width = Math.max(320, right-left+padding*2), height = Math.max(240, bottom-top+padding*2);
  return [(left+right-width)/2, (top+bottom-height)/2, width, height];
}
function resetPositions() {
  const layout = GraphLayout.layout(data.graph);
  views[0].positions = layout.positions;
  // Leave room for parallel edges and self-loops, including loops above a vertex.
  const multiplicity = new Map();
  for (const edge of data.graph.edges) {
    const pair = [edge.from, edge.to].sort((a,b) => a-b).join(':');
    multiplicity.set(pair, (multiplicity.get(pair) || 0) + 1);
  }
  const padding = Math.max(70, 90 + 18 * Math.max(0, ...multiplicity.values()));
  views[0].box = fittedBox(layout.positions, padding); views[0].baseWidth = views[0].box[2]; treeKey = '';
}
async function load(refresh = false) {
  if (loading) return;
  const requested = $('case').value;
  if (!requested) { clearSelection('Нет доступных тестов'); return; }
  loading = true;
  try {
    const next = await get('api/case?id=' + encodeURIComponent(requested));
    if ($('case').value !== requested) return;
    const encoded = JSON.stringify(next);
    if (refresh && data && encoded === signature) { $('notice').textContent = next.warnings.join('\n'); return; }
    const sameGraph = data && data.id === next.id && JSON.stringify(data.graph) === JSON.stringify(next.graph);
    stop(); data = next; signature = encoded; steps = M.steps(data.events);
    index = refresh && sameGraph ? Math.min(index, steps.length - 1) : 0;
    if (!sameGraph) { resetPositions(); $('answer').value = ''; }
    $('description').textContent = data.label;
    $('input').textContent = data.input;
    $('expected').textContent = data.expected === null ? 'Нет файла эталона' : (data.expected || '(Пустой вывод)');
    $('actual').textContent = data.actual === null ? 'Вывод не сохранён. Сохраняются несовпадения при запуске с --save-actual.' : (data.actual || '(Пустой вывод)');
    $('graph-title').textContent = `${data.name}.in · ${data.graph.n} вершин · ${data.graph.edges.length} рёбер`;
    $('notice').textContent = data.warnings.join('\n');
    $('answer-tools').hidden = $('answer').disabled = data.task !== 'task_03';
    updateCaseStatus();
    render();
  } catch (error) {
    if (!refresh) clearSelection('Граф недоступен');
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
  const key = signature + ':' + index;
  if (treeKey !== key) {
    const layout = M.traversal(data.graph, state);
    views[1].positions = layout.positions; views[1].box = layout.box; views[1].baseWidth = layout.box[2]; treeKey = key;
  }
  document.querySelectorAll('[data-view]').forEach(button => button.setAttribute('aria-pressed', String(button.dataset.view === mode)));
  $('graph-panel').hidden = mode === 'tree';
  $('tree-panel').hidden = mode === 'graph';
  $('canvases').classList.toggle('split', mode === 'both');
  $('tree-note').hidden = mode === 'graph';
  $('tree-note').textContent = 'Показаны только записанные рёбра обхода. Непосещённые вершины — в нижнем ряду. В BFS пунктир означает нетревесные рёбра; обратные рёбра DFS ведут к предку.';
  let answer = {nodes:new Set(), edges:new Set()};
  try { answer = M.answer(data.task, data[$('answer').value] || '', data.graph); } catch(error) { showError(error); }
  views.forEach((view, i) => drawGraph(view, i === 1, state, answer));
  $('values').replaceChildren();
  const labels = {queued:'В очереди',active:'Текущая',done:'Обработана',cut:'Критическая'};
  for (const v of [...views[0].positions.keys()].sort((a,b) => a-b)) {
    const row = document.createElement('tr'); row.classList.toggle('current-row', state.nodes.get(v) === 'active');
    for (const value of [v, labels[state.nodes.get(v)] || 'Не посещена', [...(state.values.get(v)||[])].map(([k,val])=>`${k} = ${val}`).join(', ') || '—']) {
      const cell = document.createElement('td');cell.textContent = value;row.append(cell);
    }
    $('values').append(row);
  }
  const hasSteps = steps.length > 1;
  $('counter').textContent = hasSteps ? `${index} / ${steps.length-1}` : '— / —';
  $('step-kicker').textContent = hasSteps ? `Шаг ${index}` : 'Нет записи шагов';
  $('step-label').textContent = index === 0 ? (hasSteps ? 'Исходное состояние графа' : 'Граф доступен без записи. Запустите решение с --trace-dir, чтобы увидеть шаги.') : data.events[steps[index]].message;
  $('timeline').max = steps.length-1;$('timeline').value = index;
  for (const id of ['first','previous']) $(id).disabled = index === 0;
  $('next').disabled = index === steps.length-1;
  $('play').disabled = !hasSteps; $('timeline').disabled = !hasSteps; $('speed').disabled = !hasSteps;
  updateZoom();
}
function drawGraph(view, tree, state, answer) {
  const {svg, positions, box} = view;
  svg.replaceChildren(); svg.setAttribute('viewBox',box.join(' '));
  const defs = element('defs'), marker = element('marker',{id:`arrow-${svg.id}`,viewBox:'0 0 10 10',refX:9,refY:5,markerWidth:6,markerHeight:6,orient:'auto-start-reverse'});
  marker.append(element('path',{d:'M 0 0 L 10 5 L 0 10 z',fill:'#7d8288'}));defs.append(marker);svg.append(defs);
  for (const e of data.graph.edges) {
    const role = state.roles.get(e.id) || '';
    if (tree && !role) continue;
    const d = edgePath(e, positions, tree && ['back', 'non_tree'].includes(role));
    const path = element('path',{d,class:`edge ${role} ${state.edges.get(e.id)||''} ${answer.edges.has(e.id)?'answer':''}`});
    if (data.graph.directed) path.setAttribute('marker-end',`url(#arrow-${svg.id})`);
    const hit = element('path',{d,class:'edge-hit','data-edge':e.id});
    hit.append(element('title',{},`Ребро ${e.id}: ${e.from} — ${e.to}`));svg.append(path,hit);
  }
  for (const [v, [x,y]] of positions) {
    const g = element('g',{transform:`translate(${x} ${y})`,class:`node ${state.nodes.get(v)||''} ${answer.nodes.has(v)?'answer':''}`,'data-node':v});
    g.append(element('circle',{r:20}),element('text',{},v));
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
  playbackButton(true);
  const tick = () => { if (index >= steps.length-1) return stop();index++;render();timer = setTimeout(tick,Number($('speed').value)); };
  timer = setTimeout(tick,Number($('speed').value));
};
$('case').onchange = () => { stop(); load(); };
$('answer').onchange = render;
$('view-mode').onchange = render;
for (const button of document.querySelectorAll('[data-view]')) {
  button.onclick = () => { $('view-mode').value = button.dataset.view; render(); };
}
$('inspector-toggle').onclick = () => setInspector($('inspector-content').hidden);
for (const button of document.querySelectorAll('[data-layout]')) {
  button.onclick = () => {
    $('viewer').classList.toggle('inspector-right', button.dataset.layout === 'right');
    document.querySelectorAll('[data-layout]').forEach(item => item.setAttribute('aria-pressed', String(item === button)));
  };
}
function zoom(view, factor) {
  const box = view.box;
  view.box = [box[0]+box[2]*(1-factor)/2,box[1]+box[3]*(1-factor)/2,box[2]*factor,box[3]*factor];
  view.svg.setAttribute('viewBox',view.box.join(' '));
  updateZoom();
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
    const node = e.target.closest('[data-node]');
    drag = {node:node?Number(node.dataset.node):null,p:point(e),x:e.clientX,y:e.clientY};
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
    drag = null; svg.releasePointerCapture(e.pointerId);
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
  const label = expanded ? 'Выйти из полного экрана' : 'На весь экран';
  $('fullscreen').setAttribute('aria-label', label); $('fullscreen').title = label;
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
const inspectorButtons = document.querySelectorAll('.inspector-tabs button');
for (const button of inspectorButtons) {
  button.onclick = () => {
    setInspector(true);
    for (const tab of inspectorButtons) {
      tab.setAttribute('aria-pressed', String(tab === button));
      $(tab.dataset.panel).hidden = tab !== button;
    }
  };
}
let catalogCases = [];
function selectCase(id) {
  $('case').value = id;
  $('custom-case').value = catalogCases.find(c => c.id === id && c.custom)?.id || '';
  document.querySelectorAll('.test-result').forEach(button => button.setAttribute('aria-pressed', String(button.dataset.id === id)));
  updateCaseStatus(); stop(); load();
}
function showCases() {
  const cases = catalogCases.filter(c => c.task === $('task').value);
  const builtins = cases.filter(c => !c.custom);
  $('test-summary').textContent = `${builtins.filter(c => c.status === 'ok').length} / ${builtins.length} OK`;
  $('test-results').replaceChildren();
  for (const [i, c] of builtins.entries()) {
    const button = document.createElement('button');
    button.className = 'test-result';
    button.dataset.id = c.id;
    button.dataset.status = c.status;
    button.dataset.stale = String(c.verdict === 'Нужно перезапустить');
    button.title = `${c.name}.in · ${c.verdict}`;
    button.setAttribute('aria-pressed', String(c.id === $('case').value));
    const name = document.createElement('span');
    name.textContent = c.name + '.in'; name.className = 'test-name';
    const verdict = document.createElement('span');
    verdict.textContent = c.verdict; verdict.className = 'verdict';
    const number = document.createElement('span'); number.className = 'test-index'; number.textContent = String(i+1).padStart(2,'0'); number.setAttribute('aria-hidden','true');
    const copy = document.createElement('span'); copy.className = 'test-copy'; copy.append(name, verdict);
    button.append(number, copy);
    button.onclick = () => selectCase(c.id);
    $('test-results').append(button);
  }
  $('custom-case').replaceChildren(new Option('Выберите свой тест', ''));
  for (const c of cases.filter(c => c.custom)) $('custom-case').append(new Option(c.name + '.in · ' + c.verdict, c.id));
  $('custom-case').disabled = !cases.some(c => c.custom);
  $('custom-case').value = cases.find(c => c.id === $('case').value && c.custom)?.id || '';
  if (!cases.some(c => c.id === $('case').value)) selectCase(cases[0]?.id || '');
  updateCaseStatus();
}
$('task').onchange = showCases;
$('custom-case').onchange = () => { if ($('custom-case').value) selectCase($('custom-case').value); };
let catalogSignature;
async function refreshCatalog() {
  const cases = await get('api/cases');
  const encoded = JSON.stringify(cases);
  if (encoded === catalogSignature) return;
  catalogSignature = encoded;
  const selected = $('case').value, task = $('task').value;
  catalogCases = cases;
  $('case').replaceChildren(new Option('', ''));
  for (const c of cases) $('case').append(new Option(c.name, c.id));
  $('case').value = selected;
  $('task').replaceChildren();
  const taskNames = {bfs_visualization: 'BFS · Учебный пример', task_01: 'Зависимости приложений', task_02: 'Односторонние дороги', task_03: 'Уязвимости сети', task_04: 'Джонсон · Кратчайшие пути'};
  for (const name of new Set(cases.map(c => c.task))) $('task').append(new Option(taskNames[name] || name, name));
  if (cases.some(c => c.task === task)) $('task').value = task;
  showCases();
  if (!cases.length) throw Error('Не найдены файловые тесты');
}
clearSelection('Загружаем тесты…');
refreshCatalog().catch(showError);
setInterval(async () => { try { await refreshCatalog(); await load(true); } catch(error) { showError(error); } }, 3000);
