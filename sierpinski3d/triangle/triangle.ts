/// <reference path="../typedefs/three/three.d.ts" />

interface VertexArray{
	/**
	 * the geometry can be represented as a point list
	 */	
	getVertexArray():Array<THREE.Vector3>;
}

class Pyramid implements VertexArray
{
	width: number;
	height: number;
	
	p0: THREE.Vector3;
	p1: THREE.Vector3;
	p2: THREE.Vector3;
	p3: THREE.Vector3;

	
	/**
	 * get a simple pyramid
	 */
	constructor(width:number) {
		this.width = width;
		this.height = Math.sqrt( width*width / 2);
							
		this.p0 =  new THREE.Vector3(-this.width/2, -this.height/2 , -this.width/2 ) ; 
				
		this.p1 = new THREE.Vector3(0, this.height/2 , 0 ) ; 
					
		this.p2 =  new THREE.Vector3(this.width/2, -this.height/2 , -this.width/2 ) ;
	
		this.p3  = new THREE.Vector3(0, -this.height/2 , this.width/2 ) ; 				
		
	}
	
	getVertexArray()
	{
		return new Array<THREE.Vector3>(this.p0,this.p1,this.p2,this.p3);		
	}
}


class RecursiveGenerator implements VertexArray
{
	baseGeometry:VertexArray;
	recursions:number;
	scale:number;
	vertexes: Array<THREE.Vector3>;
	/**
	 *
	 */
	constructor(baseGeometry:VertexArray,recursions:number, scale:number) {
		this.baseGeometry = baseGeometry;
		this.recursions = recursions;
		this.scale = scale;
		
		var baseArray = baseGeometry.getVertexArray();
		
		this.vertexes = new Array<THREE.Vector3>().concat(baseArray);
		
		for(var i = 0; i < this.recursions; i++)
		{
			var newVertexes = new Array<THREE.Vector3>(baseArray.length * this.vertexes.length);
			var index = 0;
			var scale = Math.pow(this.scale,i+1);
			for(var j = 0; j < this.vertexes.length; j++)
			{
				for(var k=0;k< baseArray.length; k++)
				{
					newVertexes[index] = baseArray[k].clone().multiplyScalar(scale).add( this.vertexes[j]);
					index++;
				}
			}
			
			this.vertexes = newVertexes;
		}
		
	}
	
	getVertexArray()
	{
		return this.vertexes;		
	}
}