import { test } from 'node:test';
import assert from 'node:assert/strict';
import { spawnSync } from 'node:child_process';
import { mkdtemp, mkdir, readFile, readdir, rm, writeFile } from 'node:fs/promises';
import { join } from 'node:path';
import { tmpdir } from 'node:os';
import { REPO, parseGraph, parseTrace } from '../server.ts';
function run(command: string, args: string[], input?: string, env = process.env) {
  const result=spawnSync(command,args,{cwd:REPO,encoding:'utf8',input,env,timeout:60000});
  assert.equal(result.status,0,`${command}: ${result.error || result.stderr || result.stdout}`);
  return result.stdout;
}
test('BFS CLI answers agree with fixtures in both recording modes; runner records each case',async()=>{
  const root=await mkdtemp(join(tmpdir(),'graph-bfs-'));
  try {
    const build=join(root,'build'),folder=join(build,'examples','bfs_visualization');
    await mkdir(folder,{recursive:true});
    const executable=join(folder,'bfs_visualization'),plain=join(root,'plain');
    const source=join(REPO,'examples/bfs_visualization');
    run('c++',['-std=c++17','-Ilib/src',join(source,'src/main.cpp'),join(source,'src/bfs.cpp'),'-o',executable]);
    run('c++',['-std=c++17','-Ilib/src',join(source,'src/plain.cpp'),join(source,'src/bfs.cpp'),'-o',plain]);
    const tests=join(source,'tests');
    const traces=join(root,'traces');
    const report=run('python3',['scripts/run_cases.py','--example','bfs_visualization','--build-dir',build,'--trace-dir',traces]);
    assert.match(report,/chain: OK/);assert.match(report,/parallel_loop: OK/);
    for (const file of (await readdir(tests)).filter(f=>f.endsWith('.in'))) {
      const input=await readFile(join(tests,file),'utf8');
      const expected=await readFile(join(tests,file.replace('.in','.out')),'utf8');
      assert.equal(run(plain,[],input),expected);
      const trace=await readFile(join(traces,'bfs_visualization',file.replace('.in','.jsonl')),'utf8');
      const parsed=parseTrace(trace,parseGraph(input,false,1));
      assert.equal(parsed.warnings.length,0);assert.ok(parsed.events.some(e=>e.type==='step'));
    }
    // Ordinary runs explicitly remove an inherited GRAPH_TRACE destination.
    const marker=join(root,'do-not-touch');await writeFile(marker,'sentinel');
    run('python3',['scripts/run_cases.py','--example','bfs_visualization','--build-dir',build],undefined,{...process.env,GRAPH_TRACE:marker});
    assert.equal(await readFile(marker,'utf8'),'sentinel');
    // C++ JSON escaping and bounded logging, including infinite-loop-like output.
    const probe=join(root,'probe.cpp'),probeExe=join(root,'probe');
    await writeFile(probe,'#include "graph_trace.hpp"\nint main(){graph_trace::Recorder t;t.Graph(1);t.Step("quote \\\" slash \\\\ newline \\n Привет");for(int i=0;i<60000;++i)t.Step("loop");t.Finish();}\n');
    run('c++',['-std=c++17','-Ilib/src',probe,'-o',probeExe]);
    const recorded=join(root,'probe.jsonl');run(probeExe,[],undefined,{...process.env,GRAPH_TRACE:recorded});
    const parsed=parseTrace(await readFile(recorded,'utf8'),parseGraph('1 0',false,1));
    assert.match(parsed.warnings[0],/размеру/);
    assert.ok(parsed.events.some(e=>e.type==='step' && e.message.includes('Привет')));
  } finally {await rm(root,{recursive:true,force:true});}
});
