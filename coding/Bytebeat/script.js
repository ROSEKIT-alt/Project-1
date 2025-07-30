let audioCtx;
let node;

function startAudio() {
    if (audioCtx) return;

    audioCtx = new AudioContext();

    const workletBlob = new Blob([`
        class BytebeatProcessor extends AudioWorkletProcessor {
           constructor() {
              super();
              this.t = 0;
              this.step = 0;
              this.skip = sampleRate / 8000;
           }

           process(inputs, outputs) {
              const output = outputs[0];
              const channel = output[0];

              for (let i = 0; i < channel.length; i++) {
                   this.step += 1;
                   if (this.step >= this.skip) {
                       this.step = 0;
                       this.t++;
                    }

                    const t = this.t;

                    const sample = t*((t&4096?t%65536<59392?7:t&7:16)+(1&t>>14))>>(3&-t>>(t&2048?2:10))|t>>(t&16384?t&4096?10:3:2)
                    channel[i] = (sample & 255) / 128 - 1;
                }

                return true;
            }
        }

        registerProcessor("bytebeat", BytebeatProcessor);
    `], { type: "application/javascript" });

    const workletURL = URL.createObjectURL(workletBlob);

    audioCtx.audioWorklet.addModule(workletURL).then(() => {
        node = new AudioWorkletNode(audioCtx, "bytebeat");
        node.connect(audioCtx.destination);
    });
}

function stopAudio() {
    if (node) {
        node.disconnect();
        node = null;
    }

    if (audioCtx) {
        audioCtx.close();
        audioCtx = null;
    }
}

document.getElementById("startBtn").addEventListener("click", startAudio);
document.getElementById("stopBtn").addEventListener("click", stopAudio);