#include <fcntl.h>
#include <node_api.h>
#include <semaphore.h>

void cleanup_semaphore(napi_env env, void* finalize_data, void* finalize_hint)
{
    (void)finalize_hint;

    sem_t* semaphore = (sem_t*)finalize_data;

    if (finalize_data != NULL && semaphore != SEM_FAILED) {
        sem_close(semaphore);
    }
}

napi_value f_sem_open(napi_env env, napi_callback_info info)
{
    napi_value returnValue;
    napi_get_undefined(env, &returnValue);

    napi_value thisarg;
    napi_value argv[2];
    size_t argc = 2;

    if (napi_get_cb_info(env, info, &argc, argv, &thisarg, NULL) != napi_ok) {
        return returnValue;
    }

    char semname[33] = "";
    size_t semname_length = 0;

    if (napi_get_value_string_latin1(env, argv[0], semname, sizeof(semname), &semname_length) != napi_ok) {
        return returnValue;
    }

    int32_t default_value = 0;

    if (napi_get_value_int32(env, argv[1], &default_value) != napi_ok) {
        return returnValue;
    }

    sem_t* semaphore = sem_open(semname, O_CREAT, 0600, default_value);

    napi_create_external(env, (void*)semaphore, cleanup_semaphore, NULL, &returnValue);

    return returnValue;
}

napi_value f_sem_wait(napi_env env, napi_callback_info info)
{
    napi_value returnValue;
    napi_get_boolean(env, false, &returnValue);

    napi_value thisarg;
    napi_value argv[1];
    size_t argc = 1;

    if (napi_get_cb_info(env, info, &argc, argv, &thisarg, NULL) != napi_ok) {
        return returnValue;
    }

    void* sem_ptr = NULL;

    if (napi_get_value_external(env, argv[0], &sem_ptr) != napi_ok) {
        return returnValue;
    }

    sem_t* semaphore = (sem_t*)sem_ptr;

    sem_wait(semaphore);

    napi_get_boolean(env, true, &returnValue);

    return returnValue;
}

napi_value f_sem_post(napi_env env, napi_callback_info info)
{
    napi_value returnValue;
    napi_get_boolean(env, false, &returnValue);

    napi_value thisarg;
    napi_value argv[1];
    size_t argc = 1;

    if (napi_get_cb_info(env, info, &argc, argv, &thisarg, NULL) != napi_ok) {
        return returnValue;
    }

    void* sem_ptr = NULL;

    if (napi_get_value_external(env, argv[0], &sem_ptr) != napi_ok) {
        return returnValue;
    }

    sem_t* semaphore = (sem_t*)sem_ptr;

    sem_post(semaphore);

    napi_get_boolean(env, true, &returnValue);

    return returnValue;
}

napi_value init(napi_env env, napi_value ret)
{
    napi_value sem_open_v;

    if (napi_create_function(env, "sem_open", sizeof("sem_open"), f_sem_open, nullptr, &sem_open_v) != napi_ok) {
        return ret;
    }

    if (napi_set_named_property(env, ret, "sem_open", sem_open_v) != napi_ok) {
        return ret;
    }

    napi_value sem_wait_v;

    if (napi_create_function(env, "sem_wait", sizeof("sem_wait"), f_sem_wait, nullptr, &sem_wait_v) != napi_ok) {
        return ret;
    }

    if (napi_set_named_property(env, ret, "sem_wait", sem_wait_v) != napi_ok) {
        return ret;
    }

    napi_value sem_post_v;

    if (napi_create_function(env, "sem_post", sizeof("sem_post"), f_sem_post, nullptr, &sem_post_v) != napi_ok) {
        return ret;
    }

    if (napi_set_named_property(env, ret, "sem_post", sem_post_v) != napi_ok) {
        return ret;
    }

    return ret;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init);
