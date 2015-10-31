// Type definitions for three.js (FlyControls.js)
// Project: https://github.com/mrdoob/three.js/blob/master/examples/js/controls/OrbitControls.js
// Definitions by: Pauli Rikula
// Definitions

/// <reference path="./three.d.ts" />

declare module THREE {
    class FlyControls {
        constructor(object:Camera, domElement?:HTMLElement);
		
		update(delta:number): void;
		
		dispose(): void;
	}
}