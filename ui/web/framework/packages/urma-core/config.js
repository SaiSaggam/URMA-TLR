/**
 * Meteorhacks subscription manager configuration.
 * @type {{cacheLimit: number, expireIn: number}}
 */
export default subscriptionCacheConfig = {
    // maximum number of cache subscriptions
    cacheLimit: 30,
    // maximum time (minutes) before inactive subscription expires
    expireIn: 10
};
