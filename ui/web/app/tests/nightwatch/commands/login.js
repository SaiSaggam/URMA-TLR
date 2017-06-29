exports.command = function(username, password) {
  
  this   
      .waitForElementVisible(".login", 5000)
      .setValue(".form-control[type=text]",username)
      .setValue(".form-control[type=password]",password)
      .click(".box>button")
      
  return this;
      
  };
