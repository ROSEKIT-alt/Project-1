function encodePNG(width, height, rgba) {
   function u32be(n) {
      return [(n >>> 24) & 0xFF, (n >>> 16) & 0xFF, (n >>> 8) & 0xFF, n & 0xFF];
   }

   function u16le(n) {
      return [n & 0xFF, (n >>> 8) & 0xFF];
   }

   const crcTable = (function () {
      const table = new Uint8Array(256);
      for (let i = 0; i < 256; i++) {
         let c = i;
         for (let j = 0; j < 8; j++) {
            c = (c & 1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1);
         }
         table[i] = c >>> 0;
      }
      return table;
   })();

   function crc32(bytes, start = 0, len = bytes.length) {
      let c = 0xFFFFFFFF;
      for (let i = start; i < start + len; i++) {
         c = crcTable[(c ^ bytes[i]) & 0xFF] ^ (c >>> 8);
      }
      return (c ^ 0xFFFFFFFF) >>> 0;
   }

   function adler32(bytes, start = 0, len = bytes.length) {
      const MOD_ADLER = 65521;
      let a = 1;
      let b = 0;
      const end = start + len;
      for (let i = start; i < end; i++) {
         a = (a + (bytes[i] & 0xFF)) % MOD_ADLER;
         b = (b + a) % MOD_ADLER;
      }
      return ((b << 16) | a) >>> 0;
   }

   function makeChunk(typeStr, dataBytes) {
      const typeBytes = new Uint8Array([
          typeStr.charCodeAt(0),
          typeStr.charCodeAt(1),
          typeStr.charCodeAt(2),
          typeStr.charCodeAt(3)
      ]);
      const lenBytes = u32be(dataBytes.length);
      const chunkAll = new Uint8Array(4 + 4 + dataBytes.length + 4);
      chunkAll.set(lenBytes, 0);
      chunkAll.set(typeBytes, 4);
      chunkAll.set(dataBytes, 8);
      const crc = crc32(chunkAll, 4, 4 + dataBytes.length);
      const crcBytes = u32be(crc);
      chunkAll.set(crcBytes, 8 + dataBytes.length);
      return chunkAll;
   }

   const pngSig = new Uint8Array([ 137, 80, 78, 71, 13, 10, 26, 10 ]);

   const ihdr = new Uint8Array(13);
   ihdr.set(u32be(width), 0);
   ihdr.set(u32be(height), 4);
   ihdr[8] = 8;
   ihdr[9] = 6;
   ihdr[10] = 0;
   ihdr[11] = 0;
   ihdr[12] = 0;
   const ihdrChunk = makeChunk('IHDR', ihdr);

   const bytesPerPixel = 4;
   const rowLen = 1 + width * bytesPerPixel;
   const rawLen = rowLen * height;
   const raw = new Uint8Array(rawLen);
   for (let y = 0; y < height; y++) {
      const rowStart = y * rowLen;
      raw[rowStart] = 0;
      const srcStart = y * width * bytesPerPixel;
      raw.set(rgba.subarray(srcStart, srcStart + width * bytesPerPixel), rowStart + 1);
   }

   const zlibHeader = new Uint8Array([ 0x78, 0x01 ]);

   const maxBlock = 0xFFFF;
   const blocks = [];
   let remaining = rawLen;
   let offset = 0;
   while(remaining > 0) {
       const blockLen = Math.min(remaining, maxBlock);
       const isFinal = (remaining - blockLen === 0) ? 1 : 0;
       const headerByte = (isFinal & 1) | (0 << 1) | (0 << 2);
       const blockHeader = new Uint8Array(1 + 2 + 2);
       blockHeader[0] = headerByte;
       const lenBytes = u16le(blockLen);
       blockHeader[1] = lenBytes[0];
       blockHeader[2] = lenBytes[1];
       const nlen = (~blockLen) & 0xFFFF;
       const nlenBytes = u16le(nlen);
       blockHeader[3] = nlenBytes[0];
       blockHeader[4] = nlenBytes[1];

       const blockData = new Uint8Array(blockHeader.length + blockLen);
       blockData.set(blockHeader, 0);
       blockData.set(raw.subarray(offset, offset + blockLen), blockHeader.length);
       blocks.push(blockData);

       offset += blockLen
       remaining -= blockLen;
   }

   let totalZlibLen = zlibHeader.length + 4;
   for (let i = 0; i < blocks.length; i++) {
      totalZlibLen += blocks[i].length;
   }
   const zlib = new Uint8Array(totalZlibLen);
   let p = 0;
   zlib.set(zlibHeader, p);
   p += zlibHeader.length;
   for (let i = 0; i < blocks.length; i++) {
      zlib.set(blocks[i], p);
      p += blocks[i].length;
   }

   const adl = adler32(raw, 0, raw.length);
   zlib.set(u32be(adl), p);
   p += 4;

   const idatChunk = makeChunk('IDAT', zlib);
   const iendChunk = makeChunk('IEND', new Uint8Array(0));

   const totalLen = pngSig.length + ihdrChunk.length + idatChunk.length + iendChunk.length;
   const out = new Uint8Array(totalLen);
   let pos = 0;
   out.set(pngSig, pos);
   pos += pngSig.length;

   out.set(ihdrChunk, pos);
   pos += ihdrChunk.length;

   out.set(idatChunk, pos);
   pos += idatChunk.length;

   out.set(iendChunk, pos);
   pos += iendChunk.length;

   return out;
}