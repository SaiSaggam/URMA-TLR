import Formsy from 'formsy-react';

export const hasAtLeastOneNumberAndOneLetter = Formsy.addValidationRule('hasAtLeastOneNumberAndOneLetter', (values, value)=>{
    let matchValue = function (valString) {
        if(valString) {
            return valString.match(/^(?=.*[a-zA-Z])(?=.*[0-9])/);
        } else {
            return true;
        }
    };

    return matchValue(value);
});

export const equalsFieldOrEmpty = Formsy.addValidationRule('equalsFieldOrEmpty', (values, value, field)=>{
    if((value == values[field]) || (typeof value === 'undefined' && values[field] === '') || (typeof values[field] === 'undefined' && values === '')) {
        return true;
    } else {
        return false;
    }
});