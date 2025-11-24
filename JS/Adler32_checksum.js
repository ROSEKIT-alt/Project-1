function adler32(buf) {
   const MOD = 65521;
   let a = 1;
   let b = 0;

   for (let i = 0; i < buf.length; i++) {
      a = (a + buf[i]) % MOD;
      b = (b + a) % MOD;
   } 
   return ((b << 16) | a) >>> 0;
}