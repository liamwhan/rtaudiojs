/*
WAV Header
B = Byte
b = Bit

| Size              | Offset (bytes)    | Chunk                 | Section       |
|-------------------|-------------------|-----------------------|---------------|
| 32bits (4Bytes)   | 0                 | ChunkID               | RIFF Chunk Descriptor
32bits (4Bytes)  size
32bits (4Bytes)  type == WAVE
32bits (4Bytes)  type == "fmt"






*/