exports.command = function(errormsg) {
  
  this   

      .waitForElementVisible(".toast.toast-error",5000)
      .waitForElementVisible(".toast-message",5000)
      .assert.containsText(".toast-message",errormsg)
      .click(".toast-close-button")
      
  return this;
      
  };
