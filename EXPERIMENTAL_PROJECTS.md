# Infraestructura para Proyectos Experimentales

## Concepto

Los proyectos experimentales son ramas de desarrollo que:

- Replican la estructura completa del proyecto principal
- Mantienen aislamiento total (no tocan el proyecto principal)
- Siguen las mismas normas de build que PROMPT.md
- Pueden fusionarse con `main` cuando estén listos

## Estructura de Directorios

```
int128/                              # Proyecto principal
├── int128_experimental/             # Raíz de proyectos experimentales
│   ├── [nombre_experimento]/        # Cada experimento es una carpeta
│   │   ├── include/                 # Headers modificados
│   │   ├── include_new/             # Nuevos headers experimentales
│   │   ├── tests/                   # Tests del experimento
│   │   ├── benchs/                  # Benchmarks del experimento
│   │   ├── scripts/                 # Scripts específicos
│   │   ├── build/                   # Outputs de compilación
│   │   ├── build_temp/              # Archivos temporales
│   │   └── EXPERIMENT.md            # Descripción del experimento
│   └── README.md                    # Índice de experimentos activos
```

## Flujo de Trabajo

### 1. Crear nuevo experimento

```bash
# Desde la raíz del proyecto
python scripts/create_experiment.py <nombre_experimento>

# Esto crea:
# - int128_experimental/<nombre>/
# - Copia de archivos relevantes
# - Rama git: experimental/<nombre>
# - EXPERIMENT.md con plantilla
```

### 2. Desarrollar en el experimento

```bash
cd int128_experimental/<nombre>/
# Trabajar normalmente, siguiendo PROMPT.md
# Los builds van a int128_experimental/<nombre>/build/
```

### 3. Probar con todos los compiladores

```bash
# Desde int128_experimental/<nombre>/
python ../../scripts/test_experiment.py --all-compilers
```

### 4. Fusionar con main (cuando esté listo)

```bash
# Desde la raíz del proyecto
python scripts/merge_experiment.py <nombre_experimento>

# Esto:
# - Ejecuta todos los tests
# - Si pasan, hace merge a main
# - Archiva el experimento
```

## Reglas Obligatorias

1. **AISLAMIENTO**: El experimento NO puede modificar archivos fuera de su carpeta
2. **NORMAS**: Debe seguir PROMPT.md para naming, builds, etc.
3. **TESTS**: Debe incluir tests que verifiquen la funcionalidad
4. **DOCUMENTACIÓN**: EXPERIMENT.md debe describir:
   - Objetivo del experimento
   - Cambios propuestos
   - Estado actual
   - Criterios de éxito para merge

## Experimentos Activos

| Nombre | Descripción | Estado | Rama |
|--------|-------------|--------|------|
| (ninguno) | - | - | - |

## Experimentos Archivados

| Nombre | Descripción | Resultado | Fecha |
|--------|-------------|-----------|-------|
| (ninguno) | - | - | - |
