function u16le(n) {
   return [n & 0xFF, (n >>> 8) & 0xFF];
}

function Zlib(data) {
   const MAX = 0xFFFF;

   const header = new Uint8Array([ 0x78, 0x01 ]);
   const blocks = [];
   let offset = 0;
   let remaining = data.length;

   while(remaining > 0) {
        const blockLen = Math.min(remaining, MAX);
        const final = remaining === blockLen ? 1 : 0;

        const len = u16le(blockLen);
        const nlen = u16le(~blockLen & 0xFFFF);

        const blockHeader = new Uint8Array([
            final,
            len[0], len[1],
            nlen[0], nlen[1]
        ]);

        const block = new Uint8Array(5 + blockLen);
        block.set(blockHeader, 0);
        block.set(data.subarray(offset, offset + blockLen), 5);

        blocks.push(block);

        offset += blockLen;
        remaining -= blockLen;
   }

   const ad = adler32(data);
   const adBytes = new Uint8Array([
       (ad >>> 24) & 0xFF,
       (ad >>> 16) & 0xFF,
       (ad >>> 8) & 0xFF,
       ad & 0xFF
   ]);

   let total = header.length + adBytes.length;
   blocks.forEach(b => total += b.length);

   const out = new Uint8Array(total);
   let p = 0;

   out.set(header, p);
   p += header.length;

   blocks.forEach(b => {
        out.set(b, p);
        p += b.length;
   });
   out.set(adBytes, p);

   return out;
}