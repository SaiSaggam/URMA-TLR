const userValidations = {
    minLength:8,
    maxLength:12,
    isAlphanumeric: true};
const userValidationErrs = {
    minLength: 'Username must be at least 8 characters in length.',
    maxLength: 'Username cannot be more than 12 characters in length.',
    isAlphanumeric: 'Username may not contain special characters.'
};
const passwordValidations = {
    minLength:6,
    hasAtLeastOneNumberAndOneLetter: true
};
const passwordValidationErrs = {
    minLength: 'Username must be at least 6 characters in length.',
    hasAtLeastOneNumberAndOneLetter: 'Password must contain at least one number and one letter.'
};

export default Validations = {
    username: {
        userValidations: userValidations,
        userValidationErrs: userValidationErrs
    },
    password: {
        passwordValidations: passwordValidations,
        passwordValidationErrs: passwordValidationErrs
    }
};