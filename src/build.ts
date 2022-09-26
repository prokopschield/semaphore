import {execSync} from "child_process";
import {write} from "doge-json";
import fs from "fs";

write("./binding.gyp", {
    targets : [
        {
            target_name : "semaphore",
            sources : [ "src/semaphore.cc" ],
            libraries : [],
        },
    ],
});

process.stdout.write(execSync("clang-format -i --style=WebKit src/*"));

if (!fs.existsSync("./build")) {
    process.stdout.write(execSync("node-gyp configure"));
}

process.stdout.write(execSync("node-gyp build"));
