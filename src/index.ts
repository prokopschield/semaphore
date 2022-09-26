const internal = require("../build/Release/semaphore");

export {internal};

export class Semaphore {
    semaphore_pointer: any;

    constructor(name: string, open = true)
    {
        this.semaphore_pointer = internal.sem_open(name, Number(!open));
    }

    /**
     * returns true on success
     */
    wait()
    {
        return internal.sem_wait(this.semaphore_pointer);
    }

    /**
     * returns true on success
     */
    post()
    {
        return internal.sem_post(this.semaphore_pointer);
    }
}
