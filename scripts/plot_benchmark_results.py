#!/usr/bin/env python3
"""
plot_benchmark_results.py

Script para generar gráficos de benchmarks usando matplotlib.
Lee archivos CSV y genera gráficos comparativos de rendimiento.
"""

import argparse
import csv
import sys
from collections import defaultdict
from pathlib import Path
from typing import Dict, List

try:
    import matplotlib.pyplot as plt
    import matplotlib
    matplotlib.use('Agg')  # Backend sin GUI
    HAS_MATPLOTLIB = True
except ImportError:
    HAS_MATPLOTLIB = False
    print("WARNING: matplotlib not installed. Install with: pip install matplotlib")

try:
    import numpy as np
    HAS_NUMPY = True
except ImportError:
    HAS_NUMPY = False
    print("WARNING: numpy not installed. Install with: pip install numpy")

# Colores para terminal
class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    CYAN = '\033[0;36m'
    NC = '\033[0m'

def read_csv_results(csv_file: Path) -> List[Dict]:
    """Lee un archivo CSV de resultados."""
    results = []
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                results.append(row)
    except Exception as e:
        print(f"{Colors.RED}Error reading {csv_file}: {e}{Colors.NC}")
    return results

def plot_time_comparison(results: List[Dict], output_dir: Path):
    """Gráfico de barras: Tiempo por operación y tipo."""
    if not HAS_MATPLOTLIB:
        return
    
    print(f"{Colors.CYAN}Generating time comparison chart...{Colors.NC}")
    
    # Agrupar por operación
    by_operation = defaultdict(lambda: defaultdict(list))
    
    for result in results:
        op = result.get('Operation', '')
        typ = result.get('Type', '')
        time_ns = float(result.get('Time_ns', 0))
        
        by_operation[op][typ].append(time_ns)
    
    # Calcular promedios
    op_avg = {}
    for op, types in by_operation.items():
        op_avg[op] = {typ: sum(times) / len(times) for typ, times in types.items()}
    
    # Seleccionar operaciones más interesantes
    interesting_ops = ['addition', 'subtraction', 'multiplication', 'division', 
                      'bitwise_and', 'shift_left_8', 'comparison_eq']
    
    for op in interesting_ops:
        if op not in op_avg:
            continue
        
        fig, ax = plt.subplots(figsize=(12, 6))
        
        types = sorted(op_avg[op].keys())
        times = [op_avg[op][t] for t in types]
        
        bars = ax.bar(types, times, color=['#2196F3', '#4CAF50', '#FF9800', '#F44336', '#9C27B0'])
        
        ax.set_xlabel('Type', fontsize=12, fontweight='bold')
        ax.set_ylabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
        ax.set_title(f'Performance Comparison: {op}', fontsize=14, fontweight='bold')
        ax.grid(axis='y', alpha=0.3)
        
        # Añadir valores sobre barras
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.2f}ns',
                   ha='center', va='bottom', fontsize=9)
        
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        
        output_file = output_dir / f'time_{op}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"  {Colors.GREEN}✓{Colors.NC} {output_file.name}")

def plot_cycles_comparison(results: List[Dict], output_dir: Path):
    """Gráfico de barras: Ciclos por operación y tipo."""
    if not HAS_MATPLOTLIB:
        return
    
    print(f"{Colors.CYAN}Generating cycles comparison chart...{Colors.NC}")
    
    # Agrupar por operación
    by_operation = defaultdict(lambda: defaultdict(list))
    
    for result in results:
        op = result.get('Operation', '')
        typ = result.get('Type', '')
        cycles = float(result.get('Cycles', 0))
        
        if cycles > 0:
            by_operation[op][typ].append(cycles)
    
    # Calcular promedios
    op_avg = {}
    for op, types in by_operation.items():
        op_avg[op] = {typ: sum(cyc) / len(cyc) for typ, cyc in types.items()}
    
    # Seleccionar operaciones más interesantes
    interesting_ops = ['addition', 'subtraction', 'multiplication', 'division']
    
    for op in interesting_ops:
        if op not in op_avg or not op_avg[op]:
            continue
        
        fig, ax = plt.subplots(figsize=(12, 6))
        
        types = sorted(op_avg[op].keys())
        cycles = [op_avg[op][t] for t in types]
        
        bars = ax.bar(types, cycles, color=['#2196F3', '#4CAF50', '#FF9800', '#F44336', '#9C27B0'])
        
        ax.set_xlabel('Type', fontsize=12, fontweight='bold')
        ax.set_ylabel('CPU Cycles', fontsize=12, fontweight='bold')
        ax.set_title(f'CPU Cycles Comparison: {op}', fontsize=14, fontweight='bold')
        ax.grid(axis='y', alpha=0.3)
        
        # Añadir valores sobre barras
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.1f}',
                   ha='center', va='bottom', fontsize=9)
        
        plt.xticks(rotation=45, ha='right')
        plt.tight_layout()
        
        output_file = output_dir / f'cycles_{op}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"  {Colors.GREEN}✓{Colors.NC} {output_file.name}")

def plot_compiler_comparison(results: List[Dict], output_dir: Path):
    """Gráfico: Comparación entre compiladores (solo uint128_t)."""
    if not HAS_MATPLOTLIB:
        return
    
    print(f"{Colors.CYAN}Generating compiler comparison chart...{Colors.NC}")
    
    # Filtrar solo uint128_t
    uint128_results = [r for r in results if r.get('Type') == 'uint128_t']
    
    # Agrupar por operación y compilador
    by_op_compiler = defaultdict(lambda: defaultdict(list))
    
    for result in uint128_results:
        op = result.get('Operation', '')
        compiler = result.get('Compiler', '').split('-')[0]  # Solo nombre (GCC, Clang, MSVC)
        time_ns = float(result.get('Time_ns', 0))
        
        by_op_compiler[op][compiler].append(time_ns)
    
    # Calcular promedios
    op_compiler_avg = {}
    for op, compilers in by_op_compiler.items():
        op_compiler_avg[op] = {comp: sum(times) / len(times) for comp, times in compilers.items()}
    
    # Seleccionar operaciones
    interesting_ops = ['addition', 'multiplication', 'division', 'bitwise_and']
    
    for op in interesting_ops:
        if op not in op_compiler_avg:
            continue
        
        fig, ax = plt.subplots(figsize=(10, 6))
        
        compilers = sorted(op_compiler_avg[op].keys())
        times = [op_compiler_avg[op][c] for c in compilers]
        
        colors_map = {'GCC': '#2196F3', 'Clang': '#4CAF50', 'MSVC': '#FF9800', 'Intel': '#9C27B0'}
        bar_colors = [colors_map.get(c, '#757575') for c in compilers]
        
        bars = ax.bar(compilers, times, color=bar_colors)
        
        ax.set_xlabel('Compiler', fontsize=12, fontweight='bold')
        ax.set_ylabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
        ax.set_title(f'Compiler Comparison (uint128_t): {op}', fontsize=14, fontweight='bold')
        ax.grid(axis='y', alpha=0.3)
        
        # Añadir valores sobre barras
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.2f}ns',
                   ha='center', va='bottom', fontsize=10)
        
        plt.tight_layout()
        
        output_file = output_dir / f'compiler_{op}.png'
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
        
        print(f"  {Colors.GREEN}✓{Colors.NC} {output_file.name}")

def plot_speedup_heatmap(results: List[Dict], output_dir: Path):
    """Heatmap: Speedup relativo vs uint64_t."""
    if not HAS_MATPLOTLIB or not HAS_NUMPY:
        return
    
    print(f"{Colors.CYAN}Generating speedup heatmap...{Colors.NC}")
    
    # Calcular speedups
    by_operation = defaultdict(lambda: defaultdict(list))
    
    for result in results:
        op = result.get('Operation', '')
        typ = result.get('Type', '')
        time_ns = float(result.get('Time_ns', 0))
        
        by_operation[op][typ].append(time_ns)
    
    # Operaciones y tipos
    operations = sorted([op for op in by_operation.keys() 
                        if 'uint64_t' in by_operation[op] and len(by_operation[op]) > 1])[:10]
    types = ['uint128_t', '__uint128_t', 'boost_uint128', 'boost_gmp', 'boost_tommath']
    types = [t for t in types if any(t in by_operation[op] for op in operations)]
    
    if not operations or not types:
        print(f"  {Colors.YELLOW}⚠ Not enough data for heatmap{Colors.NC}")
        return
    
    # Matriz de speedups
    speedup_matrix = []
    
    for op in operations:
        if 'uint64_t' not in by_operation[op]:
            continue
        
        baseline = sum(by_operation[op]['uint64_t']) / len(by_operation[op]['uint64_t'])
        row = []
        
        for typ in types:
            if typ in by_operation[op]:
                avg_time = sum(by_operation[op][typ]) / len(by_operation[op][typ])
                speedup = baseline / avg_time if avg_time > 0 else 0
                row.append(speedup)
            else:
                row.append(0)
        
        speedup_matrix.append(row)
    
    if not speedup_matrix:
        return
    
    # Plot
    fig, ax = plt.subplots(figsize=(12, 8))
    
    im = ax.imshow(speedup_matrix, cmap='RdYlGn', aspect='auto', vmin=0, vmax=1.2)
    
    ax.set_xticks(np.arange(len(types)))
    ax.set_yticks(np.arange(len(operations)))
    ax.set_xticklabels(types)
    ax.set_yticklabels(operations)
    
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right", rotation_mode="anchor")
    
    # Añadir valores en celdas
    for i in range(len(operations)):
        for j in range(len(types)):
            value = speedup_matrix[i][j]
            if value > 0:
                text = ax.text(j, i, f'{value:.2f}',
                             ha="center", va="center", color="black", fontsize=8)
    
    ax.set_title('Speedup Relative to uint64_t (1.0 = equal performance)', 
                fontsize=14, fontweight='bold', pad=20)
    ax.set_xlabel('Type', fontsize=12, fontweight='bold')
    ax.set_ylabel('Operation', fontsize=12, fontweight='bold')
    
    cbar = plt.colorbar(im, ax=ax)
    cbar.set_label('Speedup', rotation=270, labelpad=20)
    
    plt.tight_layout()
    
    output_file = output_dir / 'speedup_heatmap.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    plt.close()
    
    print(f"  {Colors.GREEN}✓{Colors.NC} {output_file.name}")

def plot_operations_overview(results: List[Dict], output_dir: Path):
    """Gráfico de resumen: Todas las operaciones para uint128_t."""
    if not HAS_MATPLOTLIB:
        return
    
    print(f"{Colors.CYAN}Generating operations overview...{Colors.NC}")
    
    # Filtrar solo uint128_t
    uint128_results = [r for r in results if r.get('Type') == 'uint128_t']
    
    # Agrupar por operación
    by_operation = defaultdict(list)
    
    for result in uint128_results:
        op = result.get('Operation', '')
        time_ns = float(result.get('Time_ns', 0))
        by_operation[op].append(time_ns)
    
    # Calcular promedios y ordenar
    op_avg = {op: sum(times) / len(times) for op, times in by_operation.items()}
    sorted_ops = sorted(op_avg.items(), key=lambda x: x[1])
    
    # Tomar top 20
    top_ops = sorted_ops[:20]
    operations = [op[0] for op in top_ops]
    times = [op[1] for op in top_ops]
    
    fig, ax = plt.subplots(figsize=(14, 8))
    
    bars = ax.barh(operations, times, color='#2196F3')
    
    ax.set_xlabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
    ax.set_ylabel('Operation', fontsize=12, fontweight='bold')
    ax.set_title('uint128_t Performance Overview (Top 20 Fastest Operations)', 
                fontsize=14, fontweight='bold')
    ax.grid(axis='x', alpha=0.3)
    
    # Añadir valores
    for i, bar in enumerate(bars):
        width = bar.get_width()
        ax.text(width, bar.get_y() + bar.get_height()/2.,
               f' {width:.2f}ns',
               ha='left', va='center', fontsize=9)
    
    plt.tight_layout()
    
    output_file = output_dir / 'operations_overview.png'
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    plt.close()
    
    print(f"  {Colors.GREEN}✓{Colors.NC} {output_file.name}")

def main():
    parser = argparse.ArgumentParser(description='Generate benchmark charts')
    parser.add_argument('--input', '-i', type=str, 
                       help='Input CSV file (default: latest consolidated CSV)')
    parser.add_argument('--output', '-o', type=str, default='benchmark_results/plots',
                       help='Output directory for plots')
    
    args = parser.parse_args()
    
    if not HAS_MATPLOTLIB:
        print(f"{Colors.RED}Error: matplotlib is required{Colors.NC}")
        print("Install with: pip install matplotlib numpy")
        return 1
    
    print(f"{Colors.BLUE}========================================{Colors.NC}")
    print(f"{Colors.BLUE}Benchmark Plotting Tool{Colors.NC}")
    print(f"{Colors.BLUE}========================================{Colors.NC}\n")
    
    # Determinar archivo de entrada
    if args.input:
        input_file = Path(args.input)
    else:
        # Buscar último CSV consolidado
        results_dir = Path('benchmark_results')
        csv_files = sorted(results_dir.glob('consolidated_benchmarks_*.csv'), 
                          key=lambda x: x.stat().st_mtime, reverse=True)
        if csv_files:
            input_file = csv_files[0]
        else:
            print(f"{Colors.RED}Error: No benchmark CSV found{Colors.NC}")
            return 1
    
    if not input_file.exists():
        print(f"{Colors.RED}Error: File not found: {input_file}{Colors.NC}")
        return 1
    
    print(f"Input: {input_file}")
    
    # Crear directorio de salida
    output_dir = Path(args.output)
    output_dir.mkdir(parents=True, exist_ok=True)
    print(f"Output: {output_dir}\n")
    
    # Leer resultados
    results = read_csv_results(input_file)
    
    if not results:
        print(f"{Colors.RED}Error: No results found in CSV{Colors.NC}")
        return 1
    
    print(f"{Colors.CYAN}Loaded {len(results)} benchmark results{Colors.NC}\n")
    
    # Generar gráficos
    plot_time_comparison(results, output_dir)
    plot_cycles_comparison(results, output_dir)
    plot_compiler_comparison(results, output_dir)
    plot_speedup_heatmap(results, output_dir)
    plot_operations_overview(results, output_dir)
    
    print(f"\n{Colors.GREEN}========================================{Colors.NC}")
    print(f"{Colors.GREEN}Plotting Complete!{Colors.NC}")
    print(f"{Colors.GREEN}========================================{Colors.NC}")
    print(f"\nPlots saved to: {output_dir}")
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
