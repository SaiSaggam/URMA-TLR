
module.exports = {
  tags:['setupwizardsession'],
 
  "Set up Wizard login - Testing login sessions" : function (client) {

     var host = client.globals.device1.url;
    
        
	client
            .url(host)

            .waitForElementVisible('body', 1000)

            //open a new tab and open the device setup wizard
            .execute(function(newWindow){
                window.open("http://10.9.120.105", null, "height=800,width=600");
            }, [host])

            //login to next page in first tab
            .assert.urlContains('getting-started')
            .waitForElementVisible(".header-wrapper>header>h2>img",5000)
      
            .click(".next")
            .waitForElementVisible(".form-control[type=text]",5000)
      	    .clearValue(".form-control[type=text]")
      	    .setValue(".form-control[type=text]",client.globals.device1.username)
      	    .waitForElementVisible(".form-control[type=password]",5000)
      	    .clearValue(".form-control[type=password]")
      	    .setValue(".form-control[type=password]",client.globals.device1.password)
      	    .click(".next")
      	    .waitForElementVisible(".config-main>div>h1",5000)

            //switch to second tab and check whether its automatically logged in and if logged in logout in second tab
            .window_handles(function(result) {
                var temp = result.value[1];
                console.log(temp)
                this.switchWindow(temp);
            })
            .resizeWindow(1024, 768)
            .assert.urlContains('getting-started')
            .waitForElementVisible(".header-wrapper>header>div", 10000)
            .click(".header-wrapper>header>div")
      	    .waitForElementVisible(".welcome-getting-started>h1",5000)

            .closeWindow()
            
            //switch to first tab after logged out in second tab and check whether its logged out in first tab
            .window_handles(function(result) {
                var temp = result.value[0];
                console.log(temp)
                this.switchWindow(temp);
            })
            .waitForElementVisible(".welcome-getting-started>h1",10000)
            .click(".next")
            .waitForElementVisible(".config-main>div>div>h1",5000)
            .assert.elementPresent(".form-control[type=text]")
            .assert.elementPresent(".form-control[type=password]")
            
            .end();
    }
};
      

