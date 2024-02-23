# ModelConverter

XEN uses Assimp to import and process 3D models. While it can handle
30 or so different formats including OBJ, FBX, gLTF, etc, this process
occurs at runtime and can be very slow.

To avoid this bottleneck, Assimp provides an `assbin` format that
stores the already processed model which can then be loaded directly,
skipping the runtime processing step.

This program handles converting 3D models in to this `assbin` format.

## Usage

### Single file

```bash
$ ./ModelConverter suzanne.obj
```

### Batch convert (manifest.json)

If you want to convert more than one file at a time, you can create a
`manifest.json` file in the program root directory with the following
contents:

```json
{
  "convert": {
    "files": [
      /* Put file paths here */
    ]
  }
}
```

Then just run ModelConverter without any arguments:

```bash
$ ./ModelConverter
```

> Converted files output to the same path as the input files.