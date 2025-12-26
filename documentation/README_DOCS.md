# Generar Documentación con Doxygen

## 🚀 Inicio Rápido

### En MSYS2/Bash:
```bash
bash scripts/generate_docs.bash
```

### En Windows CMD/PowerShell:
```cmd
scripts\generate_docs.bat
```

La documentación se generará en: `documentation/generated/html/`

## 📦 Instalación de Doxygen

### MSYS2 (Windows):
```bash
pacman -S mingw-w64-ucrt-x86_64-doxygen
pacman -S mingw-w64-ucrt-x86_64-graphviz  # Para diagramas
```

### Linux (Ubuntu/Debian):
```bash
sudo apt install doxygen graphviz
```

### macOS:
```bash
brew install doxygen graphviz
```

### Windows (Chocolatey):
```cmd
choco install doxygen.install graphviz
```

## 🎨 Características de la Documentación

- ✅ **Búsqueda integrada** - Busca funciones, clases y métodos
- ✅ **Referencia API completa** - Cada función documentada
- ✅ **Gráficos de clases** - Herencia y relaciones (requiere Graphviz)
- ✅ **Ejemplos de código** - Snippets funcionales
- ✅ **Navegación intuitiva** - Árbol de archivos y módulos
- ✅ **Responsive** - Compatible con móviles y tablets

## 📂 Estructura de Documentación

```
documentation/
├── doxygen/
│   ├── pages/              # Páginas adicionales
│   │   ├── quickstart.md   # Guía de inicio rápido
│   │   ├── examples.md     # Ejemplos de uso
│   │   └── architecture.md # Arquitectura del proyecto
│   └── images/             # Imágenes y diagramas
├── generated/              # Salida generada (gitignore)
│   └── html/
│       ├── index.html      # Página principal
│       ├── search/         # Motor de búsqueda
│       └── ...
└── README_DOCS.md          # Este archivo
```

## 🔧 Configuración

El archivo `Doxyfile` en la raíz del proyecto contiene toda la configuración.

### Configuración clave:

```doxyfile
PROJECT_NAME    = "int128 - 128-bit Integer Types for C++"
OUTPUT_LANGUAGE = Spanish
EXTRACT_ALL     = YES
SOURCE_BROWSER  = YES
GENERATE_HTML   = YES
SEARCHENGINE    = YES
CLASS_DIAGRAMS  = YES
HAVE_DOT        = YES  # Requiere Graphviz
```

## 📝 Agregar Documentación al Código

### Documentar una función:

```cpp
/**
 * @brief Suma dos números de 128 bits
 * @param rhs Operando derecho
 * @return Resultado de la suma
 * @note No detecta overflow
 * @see safe_add() para suma con detección de overflow
 * 
 * @code
 * uint128_t a = 100;
 * uint128_t b = 200;
 * uint128_t sum = a + b;  // 300
 * @endcode
 */
constexpr uint128_t operator+(const uint128_t& rhs) const noexcept;
```

### Documentar una clase:

```cpp
/**
 * @file uint128_t.hpp
 * @brief Implementación de entero sin signo de 128 bits
 * 
 * Esta clase proporciona un tipo de dato de 128 bits sin signo
 * con todas las operaciones aritméticas, bitwise y comparaciones.
 * 
 * @see int128_t Para la versión con signo
 */
class uint128_t {
    // ...
};
```

## 🌐 Publicar en GitHub Pages

### Configurar GitHub Actions:

1. Crear workflow en `.github/workflows/docs.yml`
2. Generar documentación en cada push
3. Publicar en branch `gh-pages`

```yaml
name: Generate Docs
on: [push]
jobs:
  docs:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install Doxygen
        run: sudo apt-get install doxygen graphviz
      - name: Generate docs
        run: doxygen Doxyfile
      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: ./documentation/generated/html
```

## 🔍 Búsqueda Avanzada

La documentación incluye búsqueda JavaScript integrada:

- Busca por nombre de función, clase o método
- Autocompletado mientras escribes
- Resultados instantáneos
- Sin necesidad de servidor

## 💡 Tips

### Ver documentación localmente:

```bash
# Método 1: Abrir directamente
start documentation/generated/html/index.html     # Windows
open documentation/generated/html/index.html      # macOS
xdg-open documentation/generated/html/index.html  # Linux

# Método 2: Con servidor HTTP simple
cd documentation/generated/html
python -m http.server 8000
# Abrir: http://localhost:8000
```

### Regenerar solo si hay cambios:

```bash
# Doxygen es inteligente y solo regenera lo necesario
doxygen Doxyfile
```

### Ver warnings de documentación faltante:

```doxyfile
# En Doxyfile, cambiar:
WARN_IF_UNDOCUMENTED = YES
```

## 📚 Páginas Adicionales

Las páginas en `documentation/doxygen/pages/` aparecen en el menú:

- **Quick Start**: Guía de inicio rápido
- **Examples**: Ejemplos de uso completos
- **Architecture**: Arquitectura del proyecto

### Añadir nueva página:

1. Crear `documentation/doxygen/pages/mi_pagina.md`
2. Formato:

```cpp
/**
@page mi_pagina Título de Mi Página

@tableofcontents

@section seccion1 Primera Sección

Contenido aquí...

@subsection subsec1 Subsección

Más contenido...

@code{.cpp}
// Código de ejemplo
uint128_t x = 42;
@endcode

*/
```

## 🎯 Navegación

### En la documentación HTML:

- **Top bar**: Búsqueda, navegación principal
- **Sidebar izquierdo**: Árbol de archivos y clases
- **Contenido central**: Documentación
- **Sidebar derecho**: Tabla de contenidos de página actual

### Links entre páginas:

```cpp
@see other_function()       // Link a otra función
@ref other_page             // Link a otra página
@link uint128_t @endlink    // Link inline
```

## ❓ Troubleshooting

### "Doxygen not found":
```bash
# Verificar instalación
which doxygen
doxygen --version

# Reinstalar si es necesario
pacman -S mingw-w64-ucrt-x86_64-doxygen  # MSYS2
```

### "Graphviz not found":
```bash
# Los diagramas no se generarán, pero el resto sí
# Para habilitarlos:
pacman -S mingw-w64-ucrt-x86_64-graphviz
```

### "Output directory not writable":
```bash
# Crear directorio manualmente
mkdir -p documentation/generated
chmod -R 755 documentation/generated
```

## 📖 Referencias

- [Doxygen Manual](https://www.doxygen.nl/manual/)
- [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html)
- [Graphviz](https://graphviz.org/)
