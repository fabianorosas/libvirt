/*
 * virtypedparam-public.c: utility functions for dealing with virTypedParameters
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */
#include <config.h>
#include "virtypedparam.h"

#include "viralloc.h"
#include "virerror.h"
#include "virstring.h"

#define VIR_FROM_THIS VIR_FROM_NONE

/* The following APIs are public and their signature may never change. */

/**
 * virTypedParamsGet:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 *
 * Finds typed parameter called @name.
 *
 * Returns pointer to the parameter or NULL if it does not exist in @params.
 * This function does not raise an error, even when returning NULL.
 */
virTypedParameterPtr
virTypedParamsGet(virTypedParameterPtr params,
                  int nparams,
                  const char *name)
{
    size_t i;

    /* No need to reset errors, since this function doesn't report any.  */

    if (!params || !name)
        return NULL;

    for (i = 0; i < nparams; i++) {
        if (STREQ(params[i].field, name))
            return params + i;
    }

    return NULL;
}


#define VIR_TYPED_PARAM_CHECK_TYPE(check_type) \
    do { if (param->type != check_type) { \
        virReportError(VIR_ERR_INVALID_ARG, \
                       _("Invalid type '%s' requested for parameter '%s', " \
                         "actual type is '%s'"), \
                       virTypedParameterTypeToString(check_type), \
                       name, \
                       virTypedParameterTypeToString(param->type)); \
        virDispatchError(NULL); \
        return -1; \
    } } while (0)


/**
 * virTypedParamsGetInt:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its int value in @value. The
 * function fails with VIR_ERR_INVALID_ARG error if the parameter does not
 * have the expected type. By passing NULL as @value, the function may be
 * used to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetInt(virTypedParameterPtr params,
                     int nparams,
                     const char *name,
                     int *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_INT);
    if (value)
        *value = param->value.i;

    return 1;
}


/**
 * virTypedParamsGetUInt:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its unsigned int value in
 * @value. The function fails with VIR_ERR_INVALID_ARG error if the parameter
 * does not have the expected type. By passing NULL as @value, the function
 * may be used to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetUInt(virTypedParameterPtr params,
                      int nparams,
                      const char *name,
                      unsigned int *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_UINT);
    if (value)
        *value = param->value.ui;

    return 1;
}


/**
 * virTypedParamsGetLLong:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its long long int value in
 * @value. The function fails with VIR_ERR_INVALID_ARG error if the parameter
 * does not have the expected type. By passing NULL as @value, the function
 * may be used to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetLLong(virTypedParameterPtr params,
                       int nparams,
                       const char *name,
                       long long *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_LLONG);
    if (value)
        *value = param->value.l;

    return 1;
}


/**
 * virTypedParamsGetULLong:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its unsigned long long int
 * value in @value. The function fails with VIR_ERR_INVALID_ARG error if the
 * parameter does not have the expected type. By passing NULL as @value, the
 * function may be used to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetULLong(virTypedParameterPtr params,
                        int nparams,
                        const char *name,
                        unsigned long long *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_ULLONG);
    if (value)
        *value = param->value.ul;

    return 1;
}


/**
 * virTypedParamsGetDouble:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its double value in @value.
 * The function fails with VIR_ERR_INVALID_ARG error if the parameter does not
 * have the expected type. By passing NULL as @value, the function may be used
 * to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetDouble(virTypedParameterPtr params,
                        int nparams,
                        const char *name,
                        double *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_DOUBLE);
    if (value)
        *value = param->value.d;

    return 1;
}


/**
 * virTypedParamsGetBoolean:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its boolean value in @value.
 * The function fails with VIR_ERR_INVALID_ARG error if the parameter does not
 * have the expected type. By passing NULL as @value, the function may be used
 * to check presence and type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetBoolean(virTypedParameterPtr params,
                         int nparams,
                         const char *name,
                         int *value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_BOOLEAN);
    if (value)
        *value = !!param->value.b;

    return 1;
}


/**
 * virTypedParamsGetString:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @name: name of the parameter to find
 * @value: where to store the parameter's value
 *
 * Finds typed parameter called @name and store its char * value in @value.
 * The function does not create a copy of the string and the caller must not
 * free the string @value points to. The function fails with
 * VIR_ERR_INVALID_ARG error if the parameter does not have the expected type.
 * By passing NULL as @value, the function may be used to check presence and
 * type of the parameter.
 *
 * Returns 1 on success, 0 when the parameter does not exist in @params, or
 * -1 on error.
 */
int
virTypedParamsGetString(virTypedParameterPtr params,
                        int nparams,
                        const char *name,
                        const char **value)
{
    virTypedParameterPtr param;

    virResetLastError();

    if (!(param = virTypedParamsGet(params, nparams, name)))
        return 0;

    VIR_TYPED_PARAM_CHECK_TYPE(VIR_TYPED_PARAM_STRING);
    if (value)
        *value = param->value.s;

    return 1;
}


/**
 * virTypedParamsAddInt:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with int type and sets its value to @value.
 * If @params array points to NULL or to a space that is not large enough to
 * accommodate the new parameter (@maxparams < @nparams + 1), the function
 * allocates more space for it and updates @maxparams. On success, @nparams
 * is incremented by one. The function fails with VIR_ERR_INVALID_ARG error
 * if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddInt(virTypedParameterPtr *params,
                     int *nparams,
                     int *maxparams,
                     const char *name,
                     int value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_INT, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddUInt:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with unsigned int type and sets its value
 * to @value. If @params array points to NULL or to a space that is not large
 * enough to accommodate the new parameter (@maxparams < @nparams + 1), the
 * function allocates more space for it and updates @maxparams. On success,
 * @nparams is incremented by one. The function fails with VIR_ERR_INVALID_ARG
 * error if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddUInt(virTypedParameterPtr *params,
                      int *nparams,
                      int *maxparams,
                      const char *name,
                      unsigned int value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_UINT, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddLLong:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with long long int type and sets its value
 * to @value. If @params array points to NULL or to a space that is not large
 * enough to accommodate the new parameter (@maxparams < @nparams + 1), the
 * function allocates more space for it and updates @maxparams. On success,
 * @nparams is incremented by one. The function fails with VIR_ERR_INVALID_ARG
 * error if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddLLong(virTypedParameterPtr *params,
                       int *nparams,
                       int *maxparams,
                       const char *name,
                       long long value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_LLONG, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddULLong:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with unsigned long long type and sets its
 * value to @value. If @params array points to NULL or to a space that is not
 * large enough to accommodate the new parameter (@maxparams < @nparams + 1),
 * the function allocates more space for it and updates @maxparams. On success,
 * @nparams is incremented by one. The function fails with VIR_ERR_INVALID_ARG
 * error if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddULLong(virTypedParameterPtr *params,
                        int *nparams,
                        int *maxparams,
                        const char *name,
                        unsigned long long value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_ULLONG, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddDouble:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with double type and sets its value to
 * @value. If @params array points to NULL or to a space that is not large
 * enough to accommodate the new parameter (@maxparams < @nparams + 1), the
 * function allocates more space for it and updates @maxparams. On success,
 * @nparams is incremented by one. The function fails with VIR_ERR_INVALID_ARG
 * error if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddDouble(virTypedParameterPtr *params,
                        int *nparams,
                        int *maxparams,
                        const char *name,
                        double value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_DOUBLE, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddBoolean:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with boolean type and sets its value to
 * @value. If @params array points to NULL or to a space that is not large
 * enough to accommodate the new parameter (@maxparams < @nparams + 1), the
 * function allocates more space for it and updates @maxparams. On success,
 * @nparams is incremented by one. The function fails with VIR_ERR_INVALID_ARG
 * error if the parameter already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddBoolean(virTypedParameterPtr *params,
                         int *nparams,
                         int *maxparams,
                         const char *name,
                         int value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_BOOLEAN, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsAddString:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @value: the value to store into the new parameter
 *
 * Adds new parameter called @name with char * type and sets its value to
 * @value. The function creates its own copy of @value string, which needs to
 * be freed using virTypedParamsFree or virTypedParamsClear. If @params array
 * points to NULL or to a space that is not large enough to accommodate the
 * new parameter (@maxparams < @nparams + 1), the function allocates more
 * space for it and updates @maxparams. On success, @nparams is incremented
 * by one. The function fails with VIR_ERR_INVALID_ARG error if the parameter
 * already exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddString(virTypedParameterPtr *params,
                        int *nparams,
                        int *maxparams,
                        const char *name,
                        const char *value)
{
    char *str = NULL;
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (VIR_STRDUP(str, value) < 0)
        goto error;

    if (virTypedParameterAssign(*params + n, name,
                                VIR_TYPED_PARAM_STRING, str) < 0) {
        VIR_FREE(str);
        goto error;
    }

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}

/**
 * virTypedParamsAddStringList:
 * @params: array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to store values to
 * @values: the values to store into the new parameters
 *
 * Packs NULL-terminated list of strings @values into @params under the
 * key @name.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddStringList(virTypedParameterPtr *params,
                            int *nparams,
                            int *maxparams,
                            const char *name,
                            const char **values)
{
    size_t i;
    int rv = -1;

    if (!values)
        return 0;

    for (i = 0; values[i]; i++) {
        if ((rv = virTypedParamsAddString(params, nparams, maxparams,
                                          name, values[i])) < 0)
            break;
    }

    return rv;
}


/**
 * virTypedParamsAddFromString:
 * @params: pointer to the array of typed parameters
 * @nparams: number of parameters in the @params array
 * @maxparams: maximum number of parameters that can be stored in @params
 *      array without allocating more memory
 * @name: name of the parameter to find
 * @type: type of the parameter
 * @value: the value to store into the new parameter encoded as a string
 *
 * Adds new parameter called @name with the requested @type and parses its
 * value from the @value string. If the requested type is string, the function
 * creates its own copy of the @value string, which needs to be freed using
 * virTypedParamsFree or virTypedParamsClear. If @params array points to NULL
 * or to a space that is not large enough to accommodate the new parameter
 * (@maxparams < @nparams + 1), the function allocates more space for it and
 * updates @maxparams. On success, @nparams is incremented by one. The
 * function fails with VIR_ERR_INVALID_ARG error if the parameter already
 * exists in @params.
 *
 * Returns 0 on success, -1 on error.
 */
int
virTypedParamsAddFromString(virTypedParameterPtr *params,
                            int *nparams,
                            int *maxparams,
                            const char *name,
                            int type,
                            const char *value)
{
    size_t max = *maxparams;
    size_t n = *nparams;

    virResetLastError();

    if (VIR_RESIZE_N(*params, max, n, 1) < 0)
        goto error;
    *maxparams = max;

    if (virTypedParameterAssignFromStr(*params + n, name, type, value) < 0)
        goto error;

    *nparams += 1;
    return 0;

 error:
    virDispatchError(NULL);
    return -1;
}


/**
 * virTypedParamsClear:
 * @params: the array of typed parameters
 * @nparams: number of parameters in the @params array
 *
 * Frees all memory used by string parameters. The memory occupied by @params
 * is not freed; use virTypedParamsFree if you want it to be freed too.
 *
 * Returns nothing.
 */
void
virTypedParamsClear(virTypedParameterPtr params,
                    int nparams)
{
    size_t i;

    if (!params)
        return;

    for (i = 0; i < nparams; i++) {
        if (params[i].type == VIR_TYPED_PARAM_STRING)
            VIR_FREE(params[i].value.s);
    }
}


/**
 * virTypedParamsFree:
 * @params: the array of typed parameters
 * @nparams: number of parameters in the @params array
 *
 * Frees all memory used by string parameters and the memory occupied by
 * @params.
 *
 * Returns nothing.
 */
void
virTypedParamsFree(virTypedParameterPtr params,
                   int nparams)
{
    virTypedParamsClear(params, nparams);
    VIR_FREE(params);
}