WAV File Format Overview
========================


Header
------


| Field Size        | Offset (bytes)    | Contains          | Endianess  | Section               |
|-------------------|-------------------|-------------------|------------|-----------------------|
| 32bits (4Bytes)   | 0                 | ChunkID           | big        | RIFF Chunk Descriptor |
| 32bits (4Bytes)   | 4                 | ChunkSize         | little     | RIFF Chunk Descriptor |
| 32bits (4Bytes)   | 8                 | Format            | big        | RIFF Chunk Descriptor |
| 32bits (4Bytes)   | 12                | Subchunk1 ID      | big        | fmt SubChunk          |
| 32bits (4Bytes)   | 16                | Subchunk1 Size    | little     | fmt SubChunk          |

