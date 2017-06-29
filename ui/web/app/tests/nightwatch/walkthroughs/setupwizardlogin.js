module.exports = {
  tags:['setupwizardlogin'],
  "Set up Wizard login - Invalid credentials (no credentials)" : function (client) {

  
  client

      .url(client.globals.device1.url)
      .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
      //Welcome page
      .waitForElementVisible(".welcome-getting-started>h1",5000)
      .assert.containsText(".header-wrapper>header>h2>div","LR54")
      .assert.containsText(".welcome-getting-started>h1","Welcome")
      .assert.elementPresent(".wizard-content-area-left")
      .assert.elementPresent(".wizard-content-area-right>img")
      .assert.containsText(".wizard-links>a:nth-child(1)","Quick Start Guide")
      
      .assert.containsText(".wizard-links>a:nth-child(2)","User Manual")

      .click(".next")
      
      //Without credentials
      .click(".next")
      .waitForElementVisible(".toast.toast-error",5000)
      .waitForElementVisible(".toast-message",5000)
      .assert.containsText(".toast-message","invalid username or password")
      .click(".toast-close-button")

      .end();
  },

  "Set up Wizard login - Invalid credentials (invalid username)" : function (client) {
  client

      .url(client.globals.device1.url)
      .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
      .click(".next")

      //with invalid credentials
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]","notadmin")

      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]",client.globals.device1.password)
      .click(".next")
      .waitForElementVisible(".toast.toast-error",5000)
      .waitForElementVisible(".toast-message",5000)
      .assert.containsText(".toast-message","invalid username or password")
      .click(".toast-close-button")
 
      .end();
  },

"Set up Wizard login - Invalid credentials (invalid password)" : function (client) {
  client

      .url(client.globals.device1.url)
      .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
      .click(".next")

      //with invalid password
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]",client.globals.device1.username)

      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]","notadmin")
      .click(".next")
      .waitForElementVisible(".toast.toast-error",5000)
      .waitForElementVisible(".toast-message",5000)
      .assert.containsText(".toast-message","invalid username or password")
      .click(".toast-close-button")

      .end();
  },

"Set up Wizard login - valid credentials" : function (client) {
  client

      .url(client.globals.device1.url)
      .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
      .click(".next")

      //Login 
      .waitForElementVisible(".form-control[type=text]",5000)
      .clearValue(".form-control[type=text]")
      .setValue(".form-control[type=text]",client.globals.device1.username)

      .waitForElementVisible(".form-control[type=password]",5000)
      .clearValue(".form-control[type=password]")
      .setValue(".form-control[type=password]",client.globals.device1.password)
      .click(".next")

      //checking whether logged in successfully and logout fucntion
      .waitForElementVisible(".config-main>div>h1",5000)
      .assert.containsText(".config-main>div>h1","Firmware Update")
      .assert.containsText(".header-wrapper>header>div","LOGOUT")
      .click(".header-wrapper>header>div")
      .waitForElementVisible(".welcome-getting-started>h1",5000)
      .assert.elementNotPresent(".header-wrapper>header>div")
             
      .end();
  }
      
};
