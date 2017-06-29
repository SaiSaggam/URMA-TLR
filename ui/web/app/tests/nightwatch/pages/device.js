module.exports = function (client) {

  this.logout = function() {

    client

	.waitForElementVisible(".header-wrapper>header>div", 5000)
	.assert.containsText(".header-wrapper>header>div","LOGOUT")
	.click(".header-wrapper>header>div")
	.waitForElementVisible(".welcome-getting-started>h1", 5000)
	.assert.elementNotPresent(".header-wrapper>header>div")
		
	return client;
  };

  this.go_next = function() {

    client

	.waitForElementVisible(".next", 5000)
	.click(".next")
	.waitForElementVisible(".header-wrapper>header", 5000)
			
	return client;
  };
  
  this.go_back = function() {

    client

	.waitForElementVisible(".back", 5000)
	.assert.containsText(".back","BACK")
	.click(".back")
	.waitForElementVisible(".header-wrapper>header", 5000)
			
	return client;
  };
  
  this.validate_login = function() {

    client

	.waitForElementVisible(".header-wrapper>header>div", 5000)
	.assert.containsText(".header-wrapper>header>div","LOGOUT", "Checking whether Logged in or not")	
			
	return client;
  };
};

