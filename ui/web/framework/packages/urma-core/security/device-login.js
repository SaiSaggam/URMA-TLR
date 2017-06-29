
function loginToDevice(username, password, cb) {
    console.log('DeviceLoginService logging in');
    // Meteor accounts-password accepts a clear text string password. Use that format so device can authenticate, too.
    let options = {
        user: {
            username: username
        },
        password: password
    };

    Accounts.callLoginMethod({
        methodArguments: [options],
        userCallback: cb
    });

}

/**
 * Simple service for authenticating with a device directly over HTTPS (via Meteor accounts pacakge). Passes
 * username and password in the clear.
 */
export const DeviceLoginService = {
    login: loginToDevice,
    logout: Meteor.logout
};
