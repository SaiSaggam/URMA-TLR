import React from 'react';
import { Component, PropTypes } from 'react';
import reactMixin from 'react-mixin';

export default class Ethernet extends React.Component {

    constructor() {
        super();
    }

    getMeteorData() {
        let SubsManager = this.context.device.SubsManager;
        let Settings = this.context.device.Settings;

        var eth = {};


        let search = {_groupName: 'ethernet', _groupIndex: 0};
        let subscription = SubsManager.subscribe('settings', search);
        if (subscription.ready()) {
            eth = Settings.findOne(search);
        }

        return {
            eth: eth
        };
    }

    renderSettings() {
        let Settings = this.context.device.Settings;

        // Get tasks from this.data.tasks
        return <div>
            <label>IP Address</label>
            <input
                type="text"
                className="form-control"
                defaultValue={this.data.eth.IPaddr}
                onChange={(e)=>Settings.update(this.data.eth._id, {$set: {IPaddr: e.target.value}})}
                placeholder="IP Address"/>
            <label>Subnet Mask</label>
            <input
                type="text"
                className="form-control"
                defaultValue={this.data.eth.mask}
                onChange={(e)=>Settings.update(this.data.eth._id, {$set: {mask: e.target.value}})}
                placeholder="Subnet Mask"/>
            <label>Gateway</label>
            <input
                type="text"
                className="form-control"
                defaultValue={this.data.eth.gateway}
                onChange={(e)=>Settings.update(this.data.eth._id, {$set: {gateway: e.target.value}})}
                placeholder="Subnet Mask"/>
        </div>
    }

    render() {
        return (
            <div>
                <h2>Ethernet</h2>
                <p>Example Ethernet page with meteor subscription and collection updates from a form. Any changes
                in the form fields get saved immediately to the settings collection.</p>
                <form>
                    {this.data.eth._id ? this.renderSettings() : <p>Loading...</p>}
                </form>
            </div>
        );
    }
}

Ethernet.contextTypes = {
    device: React.PropTypes.object.isRequired
};

reactMixin(Ethernet.prototype, ReactMeteorData);
