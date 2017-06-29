exports.command = function() {
  
  this   
      .waitForElementVisible(".config-nav-main.open", 5000)
      .click(".box>button")
      .waitForElementVisible(".login", 5000)
  return this;
      
  };
