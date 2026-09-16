/* Shared pure model: browser playback and Node regression tests. */
(function (root) {
  const api = {
    steps(events) { return [-1, ...events.flatMap((e, i) => e.type === 'step' ? [i] : [])]; },
    state(events, end) {
      const nodes = new Map(), edges = new Map(), values = new Map(), roles = new Map();
      for (let i = 0; i <= end; i++) {
        const e = events[i];
        if (e.type === 'node') nodes.set(e.id, e.state);
        if (e.type === 'edge') {
          edges.set(e.id, e.state);
          if (['tree', 'back', 'non_tree'].includes(e.state)) roles.set(e.id, e.state);
        }
        if (e.type === 'value') {
          if (e.name === 'transpose' && e.value === 1) {
            nodes.clear(); edges.clear(); roles.clear();
            for (const vars of values.values()) vars.delete('parent');
          }
          if (!values.has(e.id)) values.set(e.id, new Map());
          values.get(e.id).set(e.name, e.value);
        }
      }
      return {nodes, edges, values, roles};
    },
    // Строит лес только из уже записанных рёбер дерева: будущие шаги не видны.
    traversal(graph, state) {
      const adjacency = new Map(), depth = new Map(), rows = [], positions = new Map();
      for (let v = graph.first; v < graph.first + graph.n; v++) adjacency.set(v, []);
      for (const e of graph.edges) {
        if (state.roles.get(e.id) !== 'tree') continue;
        adjacency.get(e.from).push(e.to); adjacency.get(e.to).push(e.from);
      }
      // Порядок появления вершин задаёт корни, независимо от порядка концов во входе.
      // finish belongs to the first pass: it must not mark a vertex as
      // discovered in the new DFS forest on the transposed graph.
      const transposed = state.values.get(graph.first)?.get('transpose') === 1;
      const visited = [...state.nodes].filter(([, status]) => status !== 'idle').map(([v]) => v);
      const discovered = [...new Set([...visited, ...(transposed ? [] : state.values.keys()),
        ...[...adjacency.keys()].filter(v => adjacency.get(v).length)])];
      const roots = discovered.filter(v => !state.values.get(v)?.has('parent'));
      for (const root of [...roots, ...discovered]) {
        if (depth.has(root)) continue;
        const queue = [root]; depth.set(root, 0);
        for (let i = 0; i < queue.length; i++) {
          const v = queue[i], level = depth.get(v);
          (rows[level] ||= []).push(v);
          for (const to of adjacency.get(v)) {
            if (!depth.has(to)) { depth.set(to, level + 1); queue.push(to); }
          }
        }
      }
      const unseen = [...adjacency.keys()].filter(v => !depth.has(v));
      if (unseen.length) rows.push(unseen);
      const width = Math.max(900, ...rows.map(row => (row.length + 1) * 70));
      const height = Math.max(570, rows.length * 100 + 100);
      rows.forEach((row, level) => row.forEach((v, i) =>
        positions.set(v, [(i + 1) * width / (row.length + 1), 90 + level * 100])));
      return {positions, box: [0, 0, width, height], unseen};
    },
    components(graph, removedNodes = new Set(), removedEdges = new Set()) {
      const adjacency = new Map();
      for (let v = graph.first; v < graph.first + graph.n; v++) {
        if (!removedNodes.has(v)) adjacency.set(v, []);
      }
      for (const e of graph.edges) {
        if (removedEdges.has(e.id) || !adjacency.has(e.from) || !adjacency.has(e.to)) continue;
        adjacency.get(e.from).push(e.to); adjacency.get(e.to).push(e.from);
      }
      const groups = new Map();
      let count = 0;
      for (const v of adjacency.keys()) {
        if (groups.has(v)) continue;
        const pending = [v]; groups.set(v, count);
        while (pending.length) {
          for (const to of adjacency.get(pending.pop())) {
            if (!groups.has(to)) { groups.set(to, count); pending.push(to); }
          }
        }
        count++;
      }
      return {count, groups};
    },
    answer(task, text, graph) {
      const nodes = new Set(), edges = new Set();
      if (task !== 'task_03' || !text.trim()) return {nodes, edges};
      const lines = text.trim().split(/\r?\n/);
      if (lines.length !== 4) throw Error('Ожидаются четыре строки ответа task_03');
      const cuts = lines[1].trim() === '-' ? [] : lines[1].trim().split(/\s+/).map(Number);
      const bridges = lines[3].trim() === '-' ? [] : lines[3].split(';').map(s => s.trim().split(/\s+/).map(Number));
      if (!/^\d+$/.test(lines[0].trim()) || !/^\d+$/.test(lines[2].trim()) || cuts.length !== Number(lines[0]) || bridges.length !== Number(lines[2])) throw Error('Количество элементов ответа не совпадает со списками');
      for (const v of cuts) {
        if (!Number.isInteger(v) || v < graph.first || v >= graph.first + graph.n) throw Error('Неизвестная вершина в ответе');
        nodes.add(v);
      }
      for (const pair of bridges) {
        if (pair.length !== 2 || pair.some(x => !Number.isInteger(x))) throw Error('Неверная пара вершин');
        const matches = graph.edges.filter(e => (e.from === pair[0] && e.to === pair[1]) || (e.from === pair[1] && e.to === pair[0]));
        if (!matches.length) throw Error('Неизвестное ребро в ответе');
        matches.forEach(e => edges.add(e.id));
      }
      return {nodes, edges};
    }
  };
  root.GraphModel = api;
  if (typeof module !== 'undefined') module.exports = api;
})(globalThis);
