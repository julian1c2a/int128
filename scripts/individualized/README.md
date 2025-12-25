# Scripts Individuales (Deprecados)

Este directorio contiene los scripts individuales originales del proyecto, que han sido **reemplazados por scripts genéricos**.

## Contenido

- **58 scripts `build_*_extracted_{tests|benchs}.bash`**: Scripts de compilación individuales
- **28 scripts `check_*_extracted_tests.bash`**: Scripts de ejecución de tests
- **30 scripts `run_*_extracted_benchs.bash`**: Scripts de ejecución de benchmarks

**Total: 116 scripts individuales**

## Estado: DEPRECADOS

Estos scripts se mantienen únicamente por **compatibilidad con flujos de trabajo existentes**, pero:

⚠️ **NO se recomienda su uso** para nuevos desarrollos  
⚠️ **NO se actualizarán** con nuevas características  
⚠️ **Pueden ser eliminados** en versiones futuras

## Alternativa Recomendada

Usa los **scripts genéricos** ubicados en `scripts/`:

### Bash (Tradicional)
```bash
# Compilar
bash scripts/build_generic.bash <type> <feature> <target> <compiler> <mode>

# Ejecutar tests
bash scripts/check_generic.bash <type> <feature> <compiler> <mode>

# Ejecutar benchmarks
bash scripts/run_generic.bash <type> <feature> <compiler> <mode>
```

### Python (Recomendado - Con entornos aislados)
```bash
# Inicialización (primera vez)
python scripts/init_project.py

# Compilar
python scripts/build_generic.py <type> <feature> <target> <compiler> <mode>

# Ejecutar tests
python scripts/check_generic.py <type> <feature> <compiler> <mode>

# Ejecutar benchmarks
python scripts/run_generic.py <type> <feature> <compiler> <mode>
```

## Ventajas de los Scripts Genéricos

✅ **Mantenimiento centralizado**: 4 archivos vs 116  
✅ **Entornos aislados** (Python): No contamina el entorno global  
✅ **Detección automática** de compiladores  
✅ **Validación de parámetros**  
✅ **Reducción del 96%** en código duplicado  
✅ **Configuraciones persistentes** en JSON  

## Migración

Si actualmente usas scripts individuales:

**Antes:**
```bash
bash scripts/build_uint128_bits_extracted_tests.bash
bash scripts/check_uint128_bits_extracted_tests.bash
```

**Ahora (Bash):**
```bash
bash scripts/build_generic.bash uint128 bits tests gcc release
bash scripts/check_generic.bash uint128 bits gcc release
```

**Ahora (Python - Recomendado):**
```bash
python scripts/build_generic.py uint128 bits tests gcc release
python scripts/check_generic.py uint128 bits gcc release
```

## Documentación Completa

Ver [PROMPT.md](../../PROMPT.md) sección "Scripts Genéricos" y "Sistema de Gestión de Entornos de Compiladores" para información detallada.

---

**Fecha de deprecación**: Diciembre 2025  
**Última actualización**: 25/12/2025
