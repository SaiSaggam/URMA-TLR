module.exports = {
  tags:['setupwizard'],
  "Set up Wizard : End-to-End testing" : function (client) {

  var url = client.globals.localhost.url;
  var user = client.globals.localhost.username;
  var pwd = client.globals.localhost.password;
  var path = client.globals.localhost.path;

  client
	.url(url)
	.waitForElementVisible(".header-wrapper>header>h2>img",5000,"Welcome page opened")
        
	.page.welcome().validate()
	.page.welcome().next()
	.page.login().validate()
/*	.page.login().invalid_nocredential("Failed")
	.page.login().invalid_invalidcredential("notadmin", pwd, "Failed")
	.page.login().invalid_invalidcredential(user, "notadmin", "Failed")
*/	.page.login().valid_credential(user, pwd)
	.page.device().validate_login()
	.page.select_connection().validate()
	.page.select_connection().select("Cellular")
	.page.cellular_config().validate()
		
 	.end();
  }
      
};

