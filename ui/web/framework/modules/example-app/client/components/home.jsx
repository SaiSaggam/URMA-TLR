import React from 'react';
import UsageExample from './usage-example.jsx';
import Urma from 'meteor/digi:urma-core';

let Authorized = Urma.Authorized;

export default class Home extends React.Component {

    render() {
        return (
            <div>
                <h2>Home Page</h2>
                <p>Content below this line should only be visible when you are logged in.</p>
                <hr/>
                <Authorized>
                    <p>This content is only available when logged in.</p>
                    <UsageExample/>
                </Authorized>
            </div>
        );
    }
}
