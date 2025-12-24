# Troubleshooting - int128 Project

## Errores de Linkado Fantasma (LNK1120)

### Síntoma
Al ejecutar `make t-full` aparecen errores de linkado:
```
fatal error LNK1120: 2 externos sin resolver
clang++: error: linker command failed with exit code 1120
```

Pero al compilar manualmente el mismo comando funciona perfectamente.

### Causa
Archivos intermedios corruptos (`.exe`, `.obj`) de compilaciones anteriores que quedaron en los directorios `build/`.

### Solución

**Opción 1: Limpieza específica de feature 't'**
```bash
make clean-t
make t-full
```

**Opción 2: Limpieza completa**
```bash
make clean-build
make t-full
```

**Opción 3: Limpieza manual**
```bash
rm -rf build/build_tests/clang build/build_tests/intel
rm -rf build/build_benchs/clang build/build_benchs/intel
make t-full
```

### Explicación Técnica

Los scripts de compilación verifican la existencia de archivos `.exe` con:
```bash
if [ -f "$OUTPUT_FILE" ]; then
    echo "✅ OK"
else
    echo "❌ FAILED"
fi
```

Si existe un `.exe` corrupto de una compilación anterior fallida, el script reporta éxito pero el archivo es inválido. Esto causa:
- **Build phase**: Script reporta ✅ OK (archivo existe)
- **Test phase**: ⚠️ Ejecutable no encontrado (archivo corrupto)

**Solución implementada**: 
- Target `make clean-t` para limpiar feature específica
- Siempre ejecutar limpieza si aparecen errores de linkado inconsistentes

### Verificación

Para confirmar que los ejecutables son válidos:
```bash
ls -lh build/build_tests/clang/debug/*.exe
./build/build_tests/clang/debug/uint128_t_extracted_tests.exe
```

Un ejecutable válido debe:
- Tener tamaño > 100KB
- Ejecutarse sin errores
- Mostrar resultados de tests

## Warnings de `localtime` Deprecado

### Síntoma
```
warning: 'localtime' is deprecated
```

### Causa
API de Windows recomienda `localtime_s` por seguridad.

### Solución
No crítico - funcionalidad no afectada. Para eliminar el warning:
```cpp
// Antes:
ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");

// Después:
#ifdef _WIN32
    struct tm timeinfo;
    localtime_s(&timeinfo, &time_t_now);
    ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
#else
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
#endif
```

---

**Última actualización**: 2025-12-24  
**Estado del proyecto**: ✅ Todos los tests pasando (GCC, Clang, MSVC, Intel)
