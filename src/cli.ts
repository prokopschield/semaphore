#!/usr/bin/env node

import argv from "@prokopschield/argv";

import {Semaphore} from ".";

const { name, state } = argv.expect([ "name", "state" ]);

function printIfFalse(success: boolean)
{
    if (!success) {
        console.error("failed");
    }

    return !success;
}

function main()
{
    if (!name || !state) {
        console.error("USAGE: semaphore <name> <state>");
        return 1;
    } else {
        const semaphore = new Semaphore(name);
        const state_c = state[0].toLowerCase();

        if (state_c === "o" || state_c === "p") {
            return printIfFalse(semaphore.post());
        } else if (state_c === "b" || state_c === "w") {
            return printIfFalse(semaphore.wait());
        } else {
            console.error("USAGE: semaphore <name> <'post' | 'wait'>");
            return 1;
        }
    }
}

const returnValue = Number(main());

process.exit(returnValue);
