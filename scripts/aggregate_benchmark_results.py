#!/usr/bin/env python3
"""
aggregate_benchmark_results.py

Script para agregar y analizar resultados de benchmarks de múltiples compiladores.
Lee los archivos CSV/JSON generados por los benchmarks y produce:
1. Un CSV consolidado con todos los resultados
2. Un JSON consolidado con todos los resultados
3. Un reporte HTML/Markdown con comparaciones visuales
4. Estadísticas de rendimiento relativo
"""

import csv
import json
import os
import sys
from collections import defaultdict
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Tuple

# Colores para terminal
class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    CYAN = '\033[0;36m'
    NC = '\033[0m'  # No Color

def find_benchmark_results(results_dir: Path) -> List[Path]:
    """Encuentra todos los archivos CSV de benchmarks en el directorio."""
    csv_files = list(results_dir.glob("uint128_benchmarks_*.csv"))
    return csv_files

def read_csv_results(csv_file: Path) -> List[Dict]:
    """Lee un archivo CSV de resultados y retorna una lista de diccionarios."""
    results = []
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                results.append(row)
    except Exception as e:
        print(f"{Colors.RED}Error reading {csv_file}: {e}{Colors.NC}")
    return results

def aggregate_results(results_dir: Path) -> Tuple[List[Dict], Dict]:
    """Agrega todos los resultados de benchmarks."""
    csv_files = find_benchmark_results(results_dir)
    
    if not csv_files:
        print(f"{Colors.YELLOW}No benchmark CSV files found in {results_dir}{Colors.NC}")
        return [], {}
    
    print(f"{Colors.BLUE}Found {len(csv_files)} benchmark files:{Colors.NC}")
    for csv_file in csv_files:
        print(f"  - {csv_file.name}")
    
    all_results = []
    compiler_stats = defaultdict(lambda: {"count": 0, "operations": set()})
    
    for csv_file in csv_files:
        results = read_csv_results(csv_file)
        all_results.extend(results)
        
        # Estadísticas por compilador
        for result in results:
            compiler = result.get('Compiler', 'Unknown')
            compiler_stats[compiler]["count"] += 1
            compiler_stats[compiler]["operations"].add(result.get('Operation', ''))
    
    return all_results, dict(compiler_stats)

def write_consolidated_csv(results: List[Dict], output_file: Path):
    """Escribe todos los resultados en un CSV consolidado."""
    if not results:
        return
    
    try:
        with open(output_file, 'w', newline='', encoding='utf-8') as f:
            fieldnames = ['Operation', 'Type', 'Compiler', 'Optimization', 
                         'Time_ns', 'Iterations', 'Ops_per_sec', 'Timestamp']
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(results)
        
        print(f"{Colors.GREEN}✓ Consolidated CSV written to: {output_file}{Colors.NC}")
    except Exception as e:
        print(f"{Colors.RED}Error writing CSV: {e}{Colors.NC}")

def write_consolidated_json(results: List[Dict], output_file: Path):
    """Escribe todos los resultados en un JSON consolidado."""
    if not results:
        return
    
    try:
        data = {
            "metadata": {
                "generated_at": datetime.now().isoformat(),
                "total_results": len(results)
            },
            "benchmark_results": results
        }
        
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2)
        
        print(f"{Colors.GREEN}✓ Consolidated JSON written to: {output_file}{Colors.NC}")
    except Exception as e:
        print(f"{Colors.RED}Error writing JSON: {e}{Colors.NC}")

def calculate_speedup(results: List[Dict]) -> Dict:
    """
    Calcula speedup relativo entre tipos.
    Usa uint64_t como baseline.
    """
    # Organizar resultados por operación y tipo
    by_operation = defaultdict(lambda: defaultdict(lambda: {'times': [], 'cycles': []}))
    
    for result in results:
        op = result.get('Operation', '')
        typ = result.get('Type', '')
        time_ns = float(result.get('Time_ns', 0))
        cycles = float(result.get('Cycles', 0))
        
        by_operation[op][typ]['times'].append(time_ns)
        by_operation[op][typ]['cycles'].append(cycles)
    
    # Calcular promedios
    speedups = {}
    for op, types in by_operation.items():
        if 'uint64_t' not in types:
            continue
        
        baseline_time = sum(types['uint64_t']['times']) / len(types['uint64_t']['times'])
        baseline_cycles = sum(types['uint64_t']['cycles']) / len(types['uint64_t']['cycles'])
        speedups[op] = {}
        
        for typ, data in types.items():
            avg_time = sum(data['times']) / len(data['times'])
            avg_cycles = sum(data['cycles']) / len(data['cycles'])
            speedup_time = baseline_time / avg_time if avg_time > 0 else 0
            speedup_cycles = baseline_cycles / avg_cycles if avg_cycles > 0 else 0
            speedups[op][typ] = {
                'time_ns': avg_time,
                'cycles': avg_cycles,
                'speedup_time': speedup_time,
                'speedup_cycles': speedup_cycles,
                'relative_performance': f"{speedup_time:.2f}x"
            }
    
    return speedups

def write_markdown_report(results: List[Dict], compiler_stats: Dict, 
                         speedups: Dict, output_file: Path):
    """Genera un reporte en Markdown con análisis de resultados."""
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write("# uint128_t Benchmark Results\n\n")
            f.write(f"**Generated:** {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
            f.write(f"**Total Results:** {len(results)}\n\n")
            
            # Estadísticas por compilador
            f.write("## Compiler Statistics\n\n")
            f.write("| Compiler | Results | Unique Operations |\n")
            f.write("|----------|---------|-------------------|\n")
            for compiler, stats in compiler_stats.items():
                ops_count = len(stats["operations"])
                f.write(f"| {compiler} | {stats['count']} | {ops_count} |\n")
            f.write("\n")
            
            # Speedup analysis
            if speedups:
                f.write("## Performance Comparison (Relative to uint64_t)\n\n")
                f.write("| Operation | Type | Avg Time (ns) | Avg Cycles | Speedup (Time) | Speedup (Cycles) |\n")
                f.write("|-----------|------|---------------|------------|----------------|------------------|\n")
                
                for op in sorted(speedups.keys()):
                    for typ in sorted(speedups[op].keys()):
                        info = speedups[op][typ]
                        f.write(f"| {op} | {typ} | {info['time_ns']:.2f} | {info['cycles']:.2f} | "
                               f"{info['speedup_time']:.2f}x | {info['speedup_cycles']:.2f}x |\n")
                f.write("\n")
            
            # Detalles por operación
            f.write("## Detailed Results by Operation\n\n")
            
            # Agrupar por operación
            by_op = defaultdict(list)
            for result in results:
                op = result.get('Operation', '')
                by_op[op].append(result)
            
            for op in sorted(by_op.keys()):
                f.write(f"### {op}\n\n")
                f.write("| Type | Compiler | Time (ns) | Cycles | Ops/sec |\n")
                f.write("|------|----------|-----------|--------|----------|\n")
                
                # Ordenar por tiempo (más rápido primero)
                sorted_results = sorted(by_op[op], 
                                       key=lambda x: float(x.get('Time_ns', 0)))
                
                for result in sorted_results:
                    typ = result.get('Type', '')
                    compiler = result.get('Compiler', '')
                    time_ns = float(result.get('Time_ns', 0))
                    cycles = float(result.get('Cycles', 0))
                    ops_per_sec = float(result.get('Ops_per_sec', 0))
                    
                    f.write(f"| {typ} | {compiler} | {time_ns:.3f} | {cycles:.2f} | "
                           f"{ops_per_sec:,.0f} |\n")
                f.write("\n")
            
            # Top performers
            f.write("## Top Performers\n\n")
            f.write("### Fastest Operations (uint128_t)\n\n")
            
            uint128_results = [r for r in results if r.get('Type') == 'uint128_t']
            fastest = sorted(uint128_results, 
                           key=lambda x: float(x.get('Time_ns', float('inf'))))[:10]
            
            f.write("| Rank | Operation | Compiler | Time (ns) | Cycles |\n")
            f.write("|------|-----------|----------|-----------|--------|\n")
            for i, result in enumerate(fastest, 1):
                op = result.get('Operation', '')
                compiler = result.get('Compiler', '')
                time_ns = float(result.get('Time_ns', 0))
                cycles = float(result.get('Cycles', 0))
                f.write(f"| {i} | {op} | {compiler} | {time_ns:.3f} | {cycles:.2f} |\n")
            f.write("\n")
            
            # Slowest operations
            f.write("### Slowest Operations (uint128_t)\n\n")
            slowest = sorted(uint128_results, 
                           key=lambda x: float(x.get('Time_ns', 0)), 
                           reverse=True)[:10]
            
            f.write("| Rank | Operation | Compiler | Time (ns) | Cycles |\n")
            f.write("|------|-----------|----------|-----------|--------|\n")
            for i, result in enumerate(slowest, 1):
                op = result.get('Operation', '')
                compiler = result.get('Compiler', '')
                time_ns = float(result.get('Time_ns', 0))
                cycles = float(result.get('Cycles', 0))
                f.write(f"| {i} | {op} | {compiler} | {time_ns:.3f} | {cycles:.2f} |\n")
            f.write("\n")
            
            # Compiler comparison
            f.write("## Compiler Comparison (uint128_t only)\n\n")
            
            # Agrupar por operación y compilador
            op_compiler = defaultdict(lambda: defaultdict(list))
            for result in uint128_results:
                op = result.get('Operation', '')
                compiler = result.get('Compiler', '')
                time_ns = float(result.get('Time_ns', 0))
                op_compiler[op][compiler].append(time_ns)
            
            # Calcular promedios
            f.write("| Operation | GCC (ns) | Clang (ns) | MSVC (ns) | Winner |\n")
            f.write("|-----------|----------|------------|-----------|--------|\n")
            
            for op in sorted(op_compiler.keys()):
                compilers = op_compiler[op]
                
                gcc_avg = sum(compilers.get('GCC', [0])) / len(compilers.get('GCC', [1]))
                clang_avg = sum(compilers.get('Clang', [0])) / len(compilers.get('Clang', [1]))
                msvc_avg = sum(compilers.get('MSVC', [0])) / len(compilers.get('MSVC', [1]))
                
                # Determinar ganador
                times = {}
                if gcc_avg > 0:
                    times['GCC'] = gcc_avg
                if clang_avg > 0:
                    times['Clang'] = clang_avg
                if msvc_avg > 0:
                    times['MSVC'] = msvc_avg
                
                winner = min(times, key=times.get) if times else "N/A"
                
                gcc_str = f"{gcc_avg:.3f}" if gcc_avg > 0 else "N/A"
                clang_str = f"{clang_avg:.3f}" if clang_avg > 0 else "N/A"
                msvc_str = f"{msvc_avg:.3f}" if msvc_avg > 0 else "N/A"
                
                f.write(f"| {op} | {gcc_str} | {clang_str} | {msvc_str} | {winner} |\n")
            f.write("\n")
        
        print(f"{Colors.GREEN}✓ Markdown report written to: {output_file}{Colors.NC}")
    
    except Exception as e:
        print(f"{Colors.RED}Error writing Markdown report: {e}{Colors.NC}")

def main():
    # Determinar directorios
    script_dir = Path(__file__).parent
    root_dir = script_dir.parent
    results_dir = root_dir / "benchmark_results"
    
    if not results_dir.exists():
        print(f"{Colors.RED}Error: benchmark_results directory not found{Colors.NC}")
        return 1
    
    print(f"{Colors.BLUE}========================================{Colors.NC}")
    print(f"{Colors.BLUE}Aggregating Benchmark Results{Colors.NC}")
    print(f"{Colors.BLUE}========================================{Colors.NC}\n")
    
    # Agregar resultados
    all_results, compiler_stats = aggregate_results(results_dir)
    
    if not all_results:
        print(f"{Colors.YELLOW}No results to aggregate{Colors.NC}")
        return 0
    
    print(f"\n{Colors.CYAN}Total results collected: {len(all_results)}{Colors.NC}\n")
    
    # Calcular speedups
    speedups = calculate_speedup(all_results)
    
    # Escribir resultados consolidados
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    
    csv_output = results_dir / f"consolidated_benchmarks_{timestamp}.csv"
    json_output = results_dir / f"consolidated_benchmarks_{timestamp}.json"
    md_output = results_dir / f"benchmark_report_{timestamp}.md"
    
    write_consolidated_csv(all_results, csv_output)
    write_consolidated_json(all_results, json_output)
    write_markdown_report(all_results, compiler_stats, speedups, md_output)
    
    print(f"\n{Colors.GREEN}========================================{Colors.NC}")
    print(f"{Colors.GREEN}Aggregation Complete!{Colors.NC}")
    print(f"{Colors.GREEN}========================================{Colors.NC}")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
