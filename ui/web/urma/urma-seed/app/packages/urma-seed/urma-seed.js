// Write your package code here!
import mainRoutes from './main-app/client/index.js';

const DeviceUI = {
    mainRoutes: mainRoutes
};

export default DeviceUI;

// Re-export all the collections
export * from './urma-seed-collections';
