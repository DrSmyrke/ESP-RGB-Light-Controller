//-----------------------------------------------------------------------------
var app = {
	debug: true,
	error: false,
	actionProcess: false,
	url: undefined,
	data: {
		mode: 0,
		ports: {},
		outs: {},
	},
	modsAvailable: [
		'Радуга',
		'Зонирование',
		'Огонь',
		'Один цвет',
		'Мигалка',
		'Гонки',
		'',
		'',
		'',
		'',
	],
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
window.onload = function(){
	printLocation();
	updateData( function(){ app.url = undefined; openPage( document.location.pathname ); } );
};



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
function sendForm( form, callback = undefined )
{
	var request = makeHttpObject();
	var formData = new FormData( form );
	request.open( form.method, form.action, true );
	request.send( formData );

	request.onreadystatechange = function(){
		if( request.readyState == 4 ){
			if( request.status == 200 ){
				if( app.debug ) console.log( 'sendForm >:', request.responseText );
				let dataObject = JSON.parse( ( request.responseText != '' ) ? request.responseText : '{}' );
				if( callback != undefined ) callback( dataObject );
			}
		}
	};
}

//-----------------------------------------------------------------------------
function openPage( url = '' )
{
	if( app.debug ) console.info( 'openPage >:', url, app.url );

	if( app.url == url ) return;

	let obj = getContentBox();
	if( obj == undefined ) return;

	let title			= '';

	switch( url ){
		case '/control':
			title = 'Контроль';
			buildControlUI();
		break;

		case '/settings':
			title = 'Настройки';
			buildSettingsUI();
		break;
	}

	app.url = url;

	if( title != '' ){
		window.history.pushState( {param: 'Value'}, title, url );
		document.title = title;
	}

	let footers = document.getElementsByTagName( 'FOOTER' );
	for( let i = 0; i < footers.length; i++ ){
		let footer = footers[ i ];
		let buttons = footer.getElementsByClassName( 'button' );
		for( let ii = 0; ii < buttons.length; ii++ ){
			let obj = buttons[ ii ];
			if( obj.lang == undefined || obj.lang == null ) continue;
			if( obj.lang == url ){
				obj.classList.add( 'selected' );
			}else{
				obj.classList.remove( 'selected' );
			}
		}
	}
}

//-----------------------------------------------------------------------------
function buildControlUI()
{
	let obj = getContentBox();
	if( obj == undefined ) return;

	let box = document.createElement( 'div' );
	box.classList = 'block';
	box.innerHTML = '';
	obj.appendChild( box );

	let modeString = document.createElement( 'div' );
	modeString.classList = 'string';
	modeString.innerHTML = '<text>Режим</text>';
	box.appendChild( modeString );
	let modeBox = document.createElement( 'select' );
	modeBox.name = 'mode';
	modeBox.value = app.data.mode;
		for( let num in app.modsAvailable ){
			if( app.modsAvailable[ num ] == '' ) continue;
			let option = document.createElement( 'option' );
			option.value = num;
			option.label = app.modsAvailable[ num ];
			if( app.data.mode == num ) option.selected = true;
			modeBox.appendChild( option );
		}
	modeBox.onchange = function(){
		changeParam( this );
		app.data.mode = this.value;
		buildControlUI();
	};
	modeString.appendChild( modeBox );

	
	//Интерфейсы режимов
	
	if( app.data.mode == 1 ){
		for(  let portNum in app.data.ports ){
			let portData = app.data.ports[ portNum ];
			for( let zoneNum in portData.zones ){
				let zoneData = portData.zones[ zoneNum ];
	
				let valuePreffix = portNum + ':' + zoneNum + ':';
				let boxCount = document.createElement( 'div' );
				boxCount.classList = 'string';
				boxCount.innerHTML = '<text>Порт ' + portNum + ' Зона ' + zoneNum + ' <i>(кол-во)</i></text><input type="number" name="zoneCount" value="' + zoneData.count + '" valuePreffix="' + valuePreffix + '" min="1" max="999" maxlength="3" size="3" onChange="changeParam( this );">';
				box.appendChild( boxCount );

				let boxColor = document.createElement( 'div' );
				boxColor.classList = 'string';
				boxColor.innerHTML = '<text>Порт ' + portNum + ' Зона ' + zoneNum + '</text><input type="color" name="zoneColor" value="#' + rgbToHex( toRgb( zoneData.color ) ) + '" valuePreffix="' + valuePreffix + '" onChange="changeParam( this );">';
				box.appendChild( boxColor );
			}
		}
	}else if( app.data.mode == 2 ){
		// let box1 = document.createElement( 'div' );
		// box1.classList = 'string';
		// box1.innerHTML = '<text>Верх</text><input type="color" name="allUp" value="#123456" onChange="changeParam( this );">';
		// box.appendChild( box1 );
		// let box2 = document.createElement( 'div' );
		// box2.classList = 'string';
		// box2.innerHTML = '<text>Низ</i></text><input type="color" name="allDown" value="#123456" onChange="changeParam( this );">';
		// box.appendChild( box2 );
	}else if( app.data.mode == 3 ){
		
	}



	let header = document.createElement( 'div' );
	header.classList = 'header';
	header.innerHTML = 'Выходы';
	obj.appendChild( header );

	let box2 = document.createElement( 'div' );
	box2.classList = 'block';
	obj.appendChild( box2 );

	for( let out in app.data.outs ){
		let data = app.data.outs[ out ];
		if( !data.hasOwnProperty( 'use' ) ) continue;
		if( !data.hasOwnProperty( 'state' ) ) continue;

		let checked = ( Number( data.state ) ) ? ' checked' : '';
		let string = document.createElement( 'div' );
		string.classList = 'string';
		string.innerHTML = '<text>Out ' + out + '</text><text><input type="checkbox" name="out' + out + 'state" onchange="changeParam( this );"' + checked + '></text>';
		box2.appendChild( string );
	}

	let sb = document.createElement( 'div' );
	sb.classList = 'button';
	sb.name = 'action';
	sb.value = 'save';
	sb.innerHTML = '<text>Сохранить</text>';
	sb.onclick = function(){ changeParam( this ); };
	obj.appendChild( sb );

	addAbout( obj );
}

//-----------------------------------------------------------------------------
function toRgb( num )
{
	num >>>= 0;
	let b = ( num & 0xFF00 ) >>> 8,
		g = ( num & 0xFF0000 ) >>> 16,
		r = ( num & 0xFF000000 ) >>> 24,
		a = ( ( num & 0xFF ) ) / 255 ;
	return { r: r, g: g, b: b };
}

//-----------------------------------------------------------------------------
function rgbToHex( rgb )
{
	var hex = [
		rgb.r.toString(16),
		rgb.g.toString(16),
		rgb.b.toString(16)
	];

	if( hex[0].length == 1 ) hex[0] = '0' + hex[0];
	if( hex[1].length == 1 ) hex[1] = '0' + hex[1];
	if( hex[2].length == 1 ) hex[2] = '0' + hex[2];

	return hex.join('');
}

//-----------------------------------------------------------------------------
function hexToRgb( hex )
{
	hex = parseInt(((hex.indexOf('#') > -1) ? hex.substring(1) : hex), 16);
	return {r: hex >> 16, g: (hex & 0x00FF00) >> 8, b: (hex & 0x0000FF)};
}

//-----------------------------------------------------------------------------
function buildSettingsUI()
{
	let obj = getContentBox();
	if( obj == undefined ) return;

	let box = document.createElement( 'div' );
	box.classList = 'block';
	box.innerHTML = '';
	obj.appendChild( box );


	for( let port in app.data.ports ){
		let data = app.data.ports[ port ];
		if( !data.hasOwnProperty( 'use' ) ) continue;
		if( !data.hasOwnProperty( 'leds' ) ) continue;

		let checked = ( Number( data.use ) ) ? ' checked' : '';
		let string = document.createElement( 'div' );
		string.classList = 'string';
		string.innerHTML = '<text>Port ' + port + '</text><text><input type="checkbox" name="port' + port + 'use" onchange="changeParam( this );"' + checked + '></text>';
		box.appendChild( string );

		let string2 = document.createElement( 'div' );
		string2.classList = 'string';
		string2.innerHTML = '<text>Port ' + port + ' <i>(кол-во)</i></text><text><input type="number" min="0" max="999" name="port' + port + 'leds" onchange="changeParam( this );" value="' + data.leds + '" size="3"></text>';
		box.appendChild( string2 );

		let string3 = document.createElement( 'div' );
		string3.classList = 'string';
		string3.innerHTML = '<text>Port ' + port + ' <i>(яркость)</i></text><text><input type="number" min="0" max="100" name="port' + port + 'bright" onchange="changeParam( this );" value="' + data.br + '" size="3"><i>%</i></text>';
		box.appendChild( string3 );
	}

	let header = document.createElement( 'div' );
	header.classList = 'header';
	header.innerHTML = 'Выходы';
	obj.appendChild( header );

	let box2 = document.createElement( 'div' );
	box2.classList = 'block';
	obj.appendChild( box2 );

	for( let out in app.data.outs ){
		let data = app.data.outs[ out ];
		if( !data.hasOwnProperty( 'use' ) ) continue;
		if( !data.hasOwnProperty( 'state' ) ) continue;

		let checked = ( Number( data.use ) ) ? ' checked' : '';
		let string = document.createElement( 'div' );
		string.classList = 'string';
		string.innerHTML = '<text>Out ' + out + '</text><text><input type="checkbox" name="out' + out + 'use" onchange="changeParam( this );"' + checked + '></text>';
		box2.appendChild( string );
	}

	let srb = document.createElement( 'div' );
	srb.classList = 'button';
	srb.name = 'action';
	srb.value = 'saveReboot';
	srb.innerHTML = '<text>Сохранить и перезагрузить</text>';
	srb.onclick = function(){ changeParam( this ); };
	obj.appendChild( srb );

	addAbout( obj );
}

//-----------------------------------------------------------------------------
function setFieldValue( name = '', value = undefined )
{
	if( app.debug ) console.log( "setFieldValue >:", name, value );

	if( name == '' ) return;
	if( value == undefined ) return;

	let find = false;

	let fields = document.getElementsByTagName( 'input' );
	for( let i = 0; i < fields.length; i++ ){
		if( fields[ i ].name == name ){
			fields[ i ].value = value;
			find = true;
			break;
		}
	}

	if( find ) return;

	let selects = document.getElementsByTagName( 'select' );
	for( let i = 0; i < selects.length; i++ ){
		if( selects[ i ].name == name ){
			selects[ i ].value = value;
			find = true;
			break;
		}
	}

	if( find ) return;

	let divs = document.getElementsByTagName( 'div' );
	for( let i = 0; i < divs.length; i++ ){
		let div = divs[ i ];
		if( div.lang == name ){
			if( div.lastElementChild.classList.contains( 'value' ) ){
				if( value.length != undefined ){
					if( value.length == 2 && value[ 0 ] == 'value' ){
						div.lastElementChild.className = 'value';
						div.lastElementChild.innerText = value[ 1 ];
					}else{
						div.lastElementChild.className = '';
						for( let num in value ) div.lastElementChild.classList.add( value[ num ] );
					}
				}
				find = true;
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
function changeParam( element )
{
	if( app.debug ) console.log( 'changeParam >:', element.name, element.value );

	let str = '';
	let value = undefined;

	if( element.type == 'checkbox' ){
		value = ( element.checked ) ? 1 : 0;
	}else if( element.type == 'color' ){
		let rgb = hexToRgb( element.value );
		value = ( ( rgb.r << 24 ) | ( rgb.g << 16 ) | ( rgb.b << 8 ) | 0 );
		// value >>>= 0;
	}else{
		value = element.value;
	}

	let valuePreffix = element.getAttribute( 'valuePreffix' );
	if( valuePreffix != null ) value = valuePreffix + value;

	str = 'param=' + element.name + '&value=' + value;

	if( value == undefined ) return;
	if( app.changeParam == element.name ) return;
	app.changeParam = element.name;

	var request = makeHttpObject();
	request.open( 'POST', '/set', true );
	request.setRequestHeader( 'Content-type','application/x-www-form-urlencoded' );
	request.send( str );

	request.onreadystatechange = function(){
		if( request.readyState == 4 ){
			app.changeParam = "";
			if( request.status == 200 ){
				if( app.debug ) console.log( 'changeParam >:', request.responseText );
				let dataObject = JSON.parse( ( request.responseText != '' ) ? request.responseText : '{}' );

				if( dataObject.hasOwnProperty( 'success' ) ){
					if( dataObject.success != "OK" ) alert( dataObject.success );
					updateData();
				}
			}
		}
	};
}

//-----------------------------------------------------------------------------
function CheckBit( reg, bit ) 
{
	reg = Number( Number( reg ).toFixed() );
	bit = Number( Number( bit ).toFixed() );
	return ( ( reg & (1<<bit ) ) != 0 ) ? 1 : 0;
}

//-----------------------------------------------------------------------------
function updateData( callback = undefined )
{
	var request = makeHttpObject();
	request.open( 'GET', '/get', true );
	request.send( null );

	request.onreadystatechange = function(){
		if( request.readyState == 4 ){
			if( request.status == 200 ){
				if( app.debug ) console.log( 'updateData >:', request.responseText );
				let dataObject = JSON.parse( ( request.responseText != '' ) ? request.responseText : '{}' );

				if( dataObject.hasOwnProperty( 'mode' ) ) app.data.mode = dataObject.mode;
				if( dataObject.hasOwnProperty( 'ports' ) ) app.data.ports = dataObject.ports;
				if( dataObject.hasOwnProperty( 'outs' ) ) app.data.outs = dataObject.outs;
				if( dataObject.hasOwnProperty( 'zones' ) ) app.data.zones.count = dataObject.zones;

				if( callback != undefined ) callback();


				// setTimeout( updateData, 5000 );
			}else{
				app.error = true;
				if( callback != undefined ) callback();
			}
		}else{
			app.error = true;
			if( callback != undefined ) callback();
		}
	};
}

//-----------------------------------------------------------------------------
function addAbout( obj = undefined )
{
	if( obj == undefined ) return;

	let aboutBox = document.createElement( 'div' );
	aboutBox.innerHTML = 'Created by <a href="http://drsmyrke.ru" target="_blank">Dr.Smyrke</a>';
	obj.appendChild( aboutBox );
}

//-----------------------------------------------------------------------------
