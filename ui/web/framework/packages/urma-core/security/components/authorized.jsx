import React from 'react';
import reactMixin from 'react-mixin';

/**
 * React component that shows/hides content based on whether a user is logged in or not.
 * TODO: future support for permissions, roles, etc.
 * @type {Authorized}
 */
export default class Authorized extends React.Component {

    getMeteorData() {
        return {
            user: Meteor.user()
        };
    }

    render() {
        return (
            <div>
                {this.data.user ? this.props.children : null }
            </div>
        )
    }
}

reactMixin(Authorized.prototype, ReactMeteorData);
