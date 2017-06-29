module.exports = function (client) {

  this.validate = function() {

    client

	.waitForElementVisible(".header-wrapper>header>h2>div", 5000)
	.assert.containsText(".header-wrapper>header>h2>div","LR54")
	.waitForElementVisible(".wizard-content-text>h1", 5000)
	.assert.containsText(".wizard-content-text>h1","Choose")
	.assert.elementPresent(".wizard-images>img")
	.assert.containsText("form","Select WAN connection type")
		
	return client;
  };

  this.select = function(type) {

    client

	.waitForElementVisible(".form-control",5000)
	.click(".form-control[value="+type+"]")
	.click(".next")
	.waitForElementVisible(".wizard-content-text",5000)
	.assert.containsText(".wizard-content-area-right", type)
				
	return client;
  };
    
};

