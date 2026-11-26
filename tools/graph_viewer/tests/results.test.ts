import { test } from 'node:test';
import assert from 'node:assert/strict';
import { mkdtemp, mkdir, writeFile, rm } from 'node:fs/promises';
import { createHash } from 'node:crypto';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { caseData, resultsCatalog } from '../server.ts';

test('results distinguish custom tests, missing runs and changed inputs or answers', async () => {
  const root = await mkdtemp(join(tmpdir(), 'viewer-results-'));
  try {
    const tests = join(root, 'task_03/tests');
    const traces = join(root, '.graph-traces/task_03');
    await mkdir(join(tests, 'custom'), {recursive: true});
    await mkdir(traces, {recursive: true});
    await writeFile(join(tests, 'one.in'), '1 0\n');
    await writeFile(join(tests, 'one.out'), '0\n');
    await writeFile(join(tests, 'custom/one.in'), '2 0\n');
    const hash = (s: string) => createHash('sha256').update(s).digest('hex');
    await writeFile(join(traces, 'results.json'), JSON.stringify([{
      id: 'task_03/tests/one.in', inputHash: hash('1 0\n'),
      expectedHash: hash('0\n'), status: 'ok', verdict: 'OK',
    }]));
    let cases = await resultsCatalog(root);
    assert.equal(cases.find(c => !c.custom)?.status, 'ok');
    assert.equal(cases.find(c => c.custom)?.status, 'not_run');
    await writeFile(join(tests, 'one.out'), '1\n');
    cases = await resultsCatalog(root);
    assert.equal(cases.find(c => !c.custom)?.verdict, 'Нужно перезапустить');
    await writeFile(join(tests, 'one.out'), '0\n');
    await writeFile(join(tests, 'one.in'), '2 0\n');
    assert.equal((await resultsCatalog(root)).find(c => !c.custom)?.status, 'not_run');
  } finally { await rm(root, {recursive: true, force: true}); }
});

test('malformed report entries cannot break the catalog or forge verdicts', async () => {
  const root = await mkdtemp(join(tmpdir(), 'viewer-report-'));
  try {
    const tests = join(root, 'task_03/tests'), traces = join(root, '.graph-traces/task_03');
    await mkdir(tests, {recursive: true}); await mkdir(traces, {recursive: true});
    // BOM and CRLF must hash the original bytes, just as the Python runner does.
    const input = '\ufeff1 0\r\n';
    await writeFile(join(tests, 'one.in'), input);
    const valid = {id:'task_03/tests/one.in', inputHash:createHash('sha256').update(input).digest('hex'), expectedHash:null, status:'ok', verdict:'invented'};
    await writeFile(join(traces, 'results.json'), JSON.stringify([null, 1, {}, {...valid, status:'unknown'}, valid]));
    const [result] = await resultsCatalog(root);
    assert.equal(result.status, 'ok'); assert.equal(result.verdict, 'OK');
    for (const malformed of ['{', '{}', '[null,1,{"status":"ok"}]']) {
      await writeFile(join(traces, 'results.json'), malformed);
      assert.equal((await resultsCatalog(root))[0].status, 'not_run');
    }
  } finally { await rm(root, {recursive:true,force:true}); }
});

test('custom, built-in and root tests with the same name read their own recording', async () => {
  const root = await mkdtemp(join(tmpdir(), 'viewer-trace-names-'));
  try {
    for (const [folder, group] of [['tests', ''], ['tests/custom', 'custom'], ['', 'root']]) {
      const inputDir = join(root, 'task_03', folder), traceDir = join(root, '.graph-traces/task_03', group);
      await mkdir(inputDir, {recursive:true}); await mkdir(traceDir, {recursive:true});
      await writeFile(join(inputDir, 'same.in'), '1 0\n');
      const trace = [{type:'header',version:1},{type:'graph',n:1,first:1,directed:false},
        {type:'step',message:group || 'builtin'},{type:'end'}];
      await writeFile(join(traceDir,'same.jsonl'), trace.map(e=>JSON.stringify(e)).join('\n')+'\n');
      const data = await caseData(['task_03',folder,'same.in'].filter(Boolean).join('/'), root);
      assert.deepEqual(data.warnings, []);
      assert.deepEqual(data.events, [{type:'step',message:group || 'builtin'}]);
    }
  } finally { await rm(root,{recursive:true,force:true}); }
});

test('absent outputs differ from valid empty output files', async () => {
  const root = await mkdtemp(join(tmpdir(), 'viewer-empty-output-'));
  try {
    const folder = join(root, 'task_03/tests');
    await mkdir(folder,{recursive:true}); await writeFile(join(folder,'one.in'),'1 0\n');
    const id = 'task_03/tests/one.in';
    const missing = await caseData(id,root);
    assert.equal(missing.expected,null); assert.equal(missing.actual,null);
    await writeFile(join(folder,'one.out'),''); await writeFile(join(folder,'one.out.actual'),'');
    const empty = await caseData(id,root);
    assert.equal(empty.expected,''); assert.equal(empty.actual,'');
  } finally { await rm(root,{recursive:true,force:true}); }
});
