/// <reference path="./typedefs/three/three.d.ts" />
/// <reference path="./typedefs/three/detector.d.ts" />



class CanvasTest {
    document: Document;
    container: HTMLDivElement;
    camera: THREE.PerspectiveCamera;
    scene: THREE.Scene;
    geometry: THREE.Geometry;
    materials: Array<THREE.PointCloudMaterial>;
    parameters: (number|number[])[][];
    renderer: THREE.WebGLRenderer;

    //stats: Stats;

    mouseX: number;
    mouseY: number;

    windowHalfX: number;
    windowHalfY: number;


    timerToken: number;

    constructor(document: Document) {

        //if (!Detector.webgl) Detector.addGetWebGLMessage();
        
        this.camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 3000);
        this.camera.position.z = 1000;

        this.initMovement();


        this.document = document;

        this.container = document.createElement('div');
        document.body.appendChild(this.container);



        this.scene = new THREE.Scene();
        this.scene.fog = new THREE.FogExp2(0x000000, 0.0007);

        this.geometry = new THREE.Geometry();
        
        this.materials = new Array<THREE.PointCloudMaterial>();

        for (var i = 0; i < 20000; i++) {

            var vertex = new THREE.Vector3();
            vertex.x = Math.random() * 2000 - 1000;
            vertex.y = Math.random() * 2000 - 1000;
            vertex.z = Math.random() * 2000 - 1000;

            this.geometry.vertices.push(vertex);
        }

        this.parameters = [
            [[1, 1, 0.5], 5],
            [[0.95, 1, 0.5], 4],
            [[0.90, 1, 0.5], 3],
            [[0.85, 1, 0.5], 2],
            [[0.80, 1, 0.5], 1]
        ];

        for (i = 0; i < this.parameters.length; i++) {

            var color = <number[]> this.parameters[i][0];
            var size = <number> this.parameters[i][1];


            var pointMaterialParameters = { size: size };
            this.materials[i] = new THREE.PointCloudMaterial(pointMaterialParameters);

            var particles = new THREE.PointCloud(this.geometry, this.materials[i]);

            particles.rotation.x = Math.random() * 6;
            particles.rotation.y = Math.random() * 6;
            particles.rotation.z = Math.random() * 6;

            this.scene.add(particles);
        }

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setPixelRatio(window.devicePixelRatio);
        this.renderer.setSize(window.innerWidth, window.innerHeight);
        this.container.appendChild(this.renderer.domElement);

        //this.stats = new Stats();
        //this.stats.domElement.style.position = 'absolute';
        //this.stats.domElement.style.top = '0px';
        //this.container.appendChild(this.stats.domElement);

        
    }

    initMovement() {

        this.mouseX = 0;
        this.mouseY = 0;

        this.windowHalfX = window.innerWidth / 2;
        this.windowHalfY = window.innerHeight / 2;
    }

    onWindowResize() {
        this.windowHalfX = window.innerWidth / 2;
        this.windowHalfY = window.innerHeight / 2;

        this.camera.aspect = window.innerWidth / window.innerHeight;
        this.camera.updateProjectionMatrix();

        this.renderer.setSize(window.innerWidth, window.innerHeight);
    }



    onDocumentMouseMove(event) {
        this.mouseX = event.clientX - this.windowHalfX;
        this.mouseY = event.clientY - this.windowHalfY;

    }

    onDocumentTouchStart(event) {
        if (event.touches.length === 1) {
            event.preventDefault();
            this.mouseX = event.touches[0].pageX - this.windowHalfX;
            this.mouseY = event.touches[0].pageY - this.windowHalfY;
        }
    }

    onDocumentTouchMove(event) {
        if (event.touches.length === 1) {
            event.preventDefault();
            this.mouseX = event.touches[0].pageX - this.windowHalfX;
            this.mouseY = event.touches[0].pageY - this.windowHalfY;
        }
    }

    

    render() {
        var time = Date.now() * 0.00005;

        this.camera.position.x += (this.mouseX - this.camera.position.x) * 0.05;
        this.camera.position.y += (- this.mouseY - this.camera.position.y) * 0.05;

        this.camera.lookAt(this.scene.position);

        for (var i = 0; i < this.scene.children.length; i++) {

            var object = this.scene.children[i];

            if (object instanceof THREE.PointCloud) {

                object.rotation.y = time * (i < 4 ? i + 1 : - (i + 1));

            }

        }

        for (i = 0; i < this.materials.length; i++) {

            var color = this.parameters[i][0];

            var h = (360 * (color[0] + time) % 360) / 360;
            this.materials[i].color.setHSL(h, color[1], color[2]);

        }

        this.renderer.render(this.scene, this.camera);

    }



}


window.onload = () => {
    var test = new CanvasTest(document);
    
    function animate() {
        requestAnimationFrame(animate);
        test.render();
    }
    
     function onWindowResize() {
        test.onWindowResize();
    }


    function onDocumentMouseMove(event) {
        test.onDocumentMouseMove( event);
    }

    function onDocumentTouchStart(event) {
        test.onDocumentTouchStart( event);
    }

    function onDocumentTouchMove(event) {
        test.onDocumentTouchMove( event);
    }
    
    
    test.document.addEventListener('mousemove', onDocumentMouseMove, false);
    test.document.addEventListener('touchstart', onDocumentTouchStart, false);
    test.document.addEventListener('touchmove', onDocumentTouchMove, false);

        //

    window.addEventListener('resize', onWindowResize, false);
    
    animate();
};