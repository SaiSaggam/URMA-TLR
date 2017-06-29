exports.command = function(username, password) {
  
  this   
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]",username)

      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]",password)

      .click(".next")
      
  return this;
      
  };
