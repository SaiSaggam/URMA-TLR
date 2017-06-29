import React from 'react';
import { Link } from 'react-router';

export default class WizardButtons extends React.Component {
    handleClickBack(){
        let onBack = this.props.back;
        if(typeof onBack === 'function'){
            this.props.back();
        } else {
            this.context.router.push(this.props.back);
        }
    }
    handleClickNext(e){
        let onNext = this.props.onNext;
        let transitionToNext = this.props.transitionToNext;

        if(typeof onNext === 'function'){
            let onNextResult = this.props.onNext(e);

            if(onNextResult.transitionToNext || typeof onNextResult.transitionToNext === 'undefined') {
                transitionToNext();
            }
        } else if (typeof onNext !== 'function') {
            transitionToNext();
        } else {
            return false;
        }
    }

    render(){
        return (
            <div className="wizard-buttons-wrapper">
                <div className="wizard-buttons">
                    {this.props.back?
                        <button className="btn-gray back" onClick={this.handleClickBack.bind(this)}>
                            <i className="fa fa-caret-left"></i>
                            Back
                        </button>:
                        null
                    }
                    <button disabled={!this.props.nextEnabled} className="next" onClick={this.handleClickNext.bind(this)}>
                        {/*Uncomment when later steps of wizard are added {this.props.next === '/'?
                            'Dashboard':
                            'Next'
                        }*/}
                        {this.props.back ?
                          'Next':
                          'Start Device Setup'
                        }
                        <i className="fa fa-caret-right"></i>
                    </button>
                </div>
            </div>

        )
    }
}

WizardButtons.contextTypes = {
    router: React.PropTypes.object.isRequired
};
