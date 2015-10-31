/// <reference path="./typedefs/three/three.d.ts" />
/// <reference path="./typedefs/three/detector.d.ts" />
/// <reference path="./typedefs/three/three-orbitcontrols.d.ts" />
/// <reference path="./typedefs/three/three-flycontrols.d.ts" />
/// <reference path="./triangle/triangle.ts" />

interface ControlWrapper{
    updateControl():void;
    dispose():void;
}


class OrbitalWrapper implements ControlWrapper
{
    control:THREE.OrbitControls;
    
    constructor(camera:THREE.Camera, domElement?:HTMLElement) {
        this.control = new THREE.OrbitControls(camera, domElement);
    }
    updateControl()
    {

        this.control.update();
    }  
    
    dispose()
    {
        this.control.dispose();
    }  
}

class FlyWrapper implements ControlWrapper
{
    timerToken: number;
    delta:number;
    control:THREE.FlyControls;
    
    /**
     *
     */
    constructor(camera:THREE.Camera, domElement?:HTMLElement) {
        this.control = new THREE.FlyControls(camera, domElement);
        this.timerToken = Date.now();
        this.delta = 0; 
    }
    
    updateControl()
    {
        var now = Date.now();
        this.delta = now - this.timerToken;
        this.timerToken  = now;
        
        this.control.update(this.delta);
    }  
    
    dispose()
    {
        this.control.dispose();
    }  
}


class CanvasTest {    
    document: Document;
    container: HTMLDivElement;
    camera: THREE.PerspectiveCamera;
    scene: THREE.Scene;
    geometry: THREE.Geometry;
    color:number[];
    material:THREE.PointCloudMaterial;
    renderer: THREE.WebGLRenderer;

    //stats: Stats;
    
    //orbitControl:THREE.OrbitControls;
    //flyControl:THREE.FlyControls;    
    //flyTimer:FlyWrapper;
    
    control:ControlWrapper;
    isFlyControlUsed:boolean;
    
    
    
    windowHalfX:number;
    windowHalfY:number;



    

    constructor(document: Document) {

        //if (!Detector.webgl) Detector.addGetWebGLMessage();
        
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 3000);
        this.camera.position.z = 1000;
        
        this.document = document;

        this.container = document.createElement('div');
        document.body.appendChild(this.container);
        
        
        
        
        this.windowHalfX = window.innerWidth / 2;
        this.windowHalfY = window.innerHeight / 2;
        
        
        //this.initMovement();

        this.scene = new THREE.Scene();
        this.scene.fog = new THREE.FogExp2(0x000000, 0.0007);

        this.geometry = new THREE.Geometry();        
        
        var pyramid = new Pyramid(1000);
        
        var generator = new RecursiveGenerator(pyramid,8,0.5);
        
        generator.getVertexArray().map( vertex => this.geometry.vertices.push(vertex) );
        
        this.color = [1, 1, 0.5]

        var pointMaterialParameters = { size: 2 };
        
        this.material = new THREE.PointCloudMaterial(pointMaterialParameters);
        this.material.color.setHSL(this.color[0], this.color[1], this.color[2]);        

        var particle = new THREE.PointCloud(this.geometry, this.material);
        
        this.scene.add(particle);        

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setPixelRatio(window.devicePixelRatio);
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        this.container.appendChild(this.renderer.domElement);                
             
        this.initControl();        
    }
    
    initControl()
    {
        this.isFlyControlUsed = false;     
        
        this.control = new OrbitalWrapper(this.camera,this.container);
        
        var t = this;
        function dbclick(){ 
          t.isFlyControlUsed = !t.isFlyControlUsed;
          t.control.dispose();
          if(t.isFlyControlUsed)
          {
              t.control = new FlyWrapper(t.camera,t.container);
          }
          else
          {
              t.control = new OrbitalWrapper(t.camera,t.container);
          }          
        };
                
        function onWindowResize(){ t.onWindowResize() }
        
        this.container.addEventListener('resize', onWindowResize, false);
        
        this.container.addEventListener('dblclick', dbclick );
    }
    

    onWindowResize() {
        this.windowHalfX = window.innerWidth / 2;
        this.windowHalfY = window.innerHeight / 2;

        this.camera.aspect = window.innerWidth / window.innerHeight;
        this.camera.updateProjectionMatrix();

        this.renderer.setSize(window.innerWidth, window.innerHeight);
    }


   
    

    render() {
        
        var time = Date.now() * 0.00005; 
        this.control.updateControl();

        for (var i = 0; i < this.scene.children.length; i++) {

            var object = this.scene.children[i];

            if (object instanceof THREE.PointCloud) {

                object.rotation.y = time * (i < 4 ? i + 1 : - (i + 1));

            }

        }
        
        var h = (360 * (this.color[0] + time) % 360) / 360;
        this.material.color.setHSL(h, this.color[1], this.color[2]);        

        this.renderer.render(this.scene, this.camera);

    }



}


window.onload = () => {
    var test = new CanvasTest(document);
    
    function animate() {
        requestAnimationFrame(animate);
        test.render();
    }

    
    animate();
};