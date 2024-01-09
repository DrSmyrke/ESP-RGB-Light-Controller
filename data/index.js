//-----------------------------------------------------------------------------
var app = {
	debug: true,
	error: false,
	actionProcess: false,
	url: undefined,
	data: {
		mode: 0,
		portsMax: 0,
		outsMax: 0,
		use_ports: 0,
		use_outs: 0,
		outsState: 0,
		leds: [],
		zonesCount: 0,
		zones: {},
		effects: {
			rainbow_speed: 0,
			rainbow_step: 0,
			fire_speed: 0,
			fire_step: 0,
			pulse_speed: 0,
			pulse_step: 0,
			masterColor: { r: 0, g: 0, b:0  },
		},
	},
	modsAvailable: [
		'Импульс',
		'Радуга',
		'Огонь',
		'Один цвет',
		'Зонирование',
		// 'Мигалка',
		// 'Гонки',
		'',
		'',
		'',
		'',
	],
	paramIDs: [
		'mode', 'u_port', 'u_out', 'out', 'action', 'zone', 'effect', 'port'
	],
	effectIDs: [
		'rainbow_speed', 'rainbow_step', 'fire_speed', 'fire_step', 'pulse_speed', 'pulse_step',
	],
	ws: undefined,
	wsAddr: '',
	reconnectAttempts: 0,
	localStorage: undefined,
};

//-----------------------------------------------------------------------------
window.onload = function(){
	printLocation();
	buildControlUI();
	buildSettingsUI();

	app.localStorage = new Storage( false );

	let data = app.localStorage.getStorageData( 'settings', '{}' );
	//
	//app.localStorage.removeFromStorage( 'sets', indx, undefined, '[]' );

	setValueFromName( 'tHost', ( data.hasOwnProperty( 'tHost' ) ) ? data.tHost : document.location.host );
	setValueFromName( 'tPort', ( data.hasOwnProperty( 'tPort' ) ) ? data.tPort : 81 );
	wsConnect();

	// updateData( function(){ app.url = undefined; openPage( document.location.pathname ); } );
};



//-----------------------------------------------------------------------------
function wsConnect()
{
	let addr = getValueFromName( 'tHost' );
	let port = getValueFromName( 'tPort' );
	
	if( addr == null || port == null ){
		setConnectionStatus( 'Params not found', 'valorange' );
		return;
	}

	app.wsAddr = 'ws://' + addr + ':' + port + '/';

	setConnectionStatus( 'Connecting to ' + addr + ':' + port + '...', 'valfiol' );
	
	if( app.ws != undefined ) app.ws.close();

	app.ws = new WebSocket( app.wsAddr );
	app.ws.binaryType = 'arraybuffer';
	app.ws.onopen = function(){
		let binary = new Uint8Array( 2 );
		// Command for get device configuration
		binary[ 0 ] = 0xF1;
		binary[ 1 ] = 0x00;
		app.ws.send( binary.buffer );
		setConnectionStatus( 'Connected', 'valgreen' );
		app.reconnectAttempts = 0;
	};
	// app.ws.onclose  = this.deinit();
	app.ws.onerror = function( event ) {
		console.error( 'WebSocket error:', event );
		disconnect();
	};
	app.ws.onmessage = function( event ){ wsOnMessage( event ); };
}

//-----------------------------------------------------------------------------
function disconnect()
{
	if( app.ws != undefined ) app.ws.close();

	setConnectionStatus( 'ERROR', 'valred' );

	if( app.reconnectAttempts < 50 ) {
		setTimeout(() => wsConnect(), 3000 );
		app.reconnectAttempts++;
	}else{
		console.log( 'Слишком много попыток соединения. Соединение невозможно, попробуйте позже' );
	}
}

//-----------------------------------------------------------------------------
function wsOnMessage( event )
{
	if( event.data instanceof ArrayBuffer ){
		// binary frame
		const view = new DataView( event.data );
		let command = view.getUint8( 0 );

		// Device configuration
		if( command == 0xA1 ){
			let offset = 1;

			app.data.leds								= [];
	
			// System state
			app.data.mode								= view.getUint8( offset++ );
			app.data.portsMax							= view.getUint8( offset++ );
			app.data.outsMax							= view.getUint8( offset++ );
			app.data.use_ports							= view.getUint8( offset++ );
			app.data.use_outs							= view.getUint8( offset++ );

			let port1 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port2 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port3 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port4 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port5 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port6 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port7 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
			let port8 = ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;

			app.data.leds.push( port1 );
			app.data.leds.push( port2 );
			app.data.leds.push( port3 );
			app.data.leds.push( port4 );
			app.data.leds.push( port5 );
			app.data.leds.push( port6 );
			app.data.leds.push( port7 );
			app.data.leds.push( port8 );

			app.data.outsState							= view.getUint8( offset++ );

			app.data.zonesCount							= view.getUint8( offset++ );

			app.data.zones								= {};
			for( let i = 0; i < app.data.zonesCount; i++ ){
				let port								= view.getUint8( offset++ );
				let start								= ( ( view.getUint8( offset++ ) << 8 ) | ( view.getUint8( offset++ ) ) );
				let count								= ( ( view.getUint8( offset++ ) << 8 ) | ( view.getUint8( offset++ ) ) );
				app.data.zones[ i ] = { port: port, start: start, count: count };
			}

			// Effects
			app.data.effects.rainbow_speed				= view.getUint8( offset++ );
			app.data.effects.rainbow_step				= view.getUint8( offset++ );
			app.data.effects.fire_speed					= view.getUint8( offset++ );
			app.data.effects.fire_step					= view.getUint8( offset++ );
			app.data.effects.pulse_speed				= view.getUint8( offset++ );
			app.data.effects.pulse_step					= view.getUint8( offset++ );
			app.data.effects.masterColor.r				= view.getUint8( offset++ );
			app.data.effects.masterColor.g				= view.getUint8( offset++ );
			app.data.effects.masterColor.b				= view.getUint8( offset++ );

			rebuildInputsOutsSettings();
			rebuildZonesSettings();
			setFieldValue( 'mode', app.data.mode );
			setFieldValue( 'rainbow_speed', app.data.effects.rainbow_speed );
			setFieldValue( 'rainbow_step', app.data.effects.rainbow_step );
			setFieldValue( 'fire_speed', app.data.effects.fire_speed );
			setFieldValue( 'fire_step', app.data.effects.fire_step );
			setFieldValue( 'pulse_speed', app.data.effects.pulse_speed );
			setFieldValue( 'pulse_step', app.data.effects.pulse_step );

			// offset += settings_size;

			// let cb = document.getElementById( 'modeCB' );
			// if( cb != undefined ) cb.checked = ( mode ) ? true : false;

			// // System flags
			// let flags_size = view.getUint8( offset++ );
			// let flags1 = view.getUint8( offset );
			// offset += flags_size;

			// let config = 'N/A';
			// if( CheckBit( flags1, 3 ) ){
			// 	if( CheckBit( flags1, 4 ) ){
			// 		config = 'broken';
			// 	}else{
			// 		config = ( CheckBit( flags1, 2 ) ) ? 'OK' : 'not available';
			// 	}
			// }else{
			// 	config = 'not found';
			// }
		}
	}else{
		console.warn( event.data );
	}
}

//-----------------------------------------------------------------------------
function openPage( url = '' )
{
	if( app.debug ) console.info( 'openPage >:', url, app.url );

	if( app.url == url ) return;

	let title			= '';
	let cb				= document.getElementById( 'controlBox' );
	let sb				= document.getElementById( 'settingsBox' );

	switch( url ){
		case '/control':
			title = 'Контроль';
			if( cb != undefined ) cb.classList.remove( 'hidden' );
			if( sb != undefined ) sb.classList.add( 'hidden' );
		break;

		case '/settings':
			title = 'Настройки';
			if( cb != undefined ) cb.classList.add( 'hidden' );
			if( sb != undefined ) sb.classList.remove( 'hidden' );
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
	let obj = document.getElementById( 'controlBox' );
	if( obj == undefined ){
		console.error( 'controlBox is undefined' );
		return undefined;
	}

	/*
	let box = document.createElement( 'div' );
	box.classList = 'block';
	obj.appendChild( box );

	

	
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
	}else if( app.data.mode == 3 ){
		let boxColor = document.createElement( 'div' );
		boxColor.classList = 'string';
		boxColor.innerHTML = '<text>Общий свет</text><input type="color" name="masterColor" value="#' + rgbToHex( toRgb( app.data.masterColor ) ) + '" onChange="changeParam( this );">';
		box.appendChild( boxColor );
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
	*/
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
	let obj = document.getElementById( 'settingsBox' );
	if( obj == undefined ){
		console.error( 'settingsBox is undefined' );
		return undefined;
	}

	addHeader( obj, 'Main' );

	let box = document.createElement( 'div' );
	box.classList = 'block';
	obj.appendChild( box );

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
	};
	addString( box, 'Mode', modeBox );



	addHeader( obj, 'IO' );
	let box2 = document.createElement( 'div' );
	box2.id = 'ioConfigureBlock';
	box2.classList = 'block';
	obj.appendChild( box2 );
	rebuildInputsOutsSettings();

	addHeader( obj, 'Zones' );
	let box3 = document.createElement( 'div' );
	box3.id = 'zonesConfigureBlock';
	box3.classList = 'block';
	obj.appendChild( box3 );
	rebuildZonesSettings();
	

	addHeader( obj, 'Effects' );
	let box4 = document.createElement( 'div' );
	box4.id = 'effectsConfigureBlock';
	box4.classList = 'block';
	obj.appendChild( box4 );

	let effectRainbowSpeedNE = createEditorElement( 'number', '', 'rainbow_speed', function(){ changeEffect( this ); }, 0, 255 );
	effectRainbowSpeedNE.size = 4;
	addString( box4, 'Rainbow speed', effectRainbowSpeedNE );
	let effectRainbowStepNE = createEditorElement( 'number', '', 'rainbow_step', function(){ changeEffect( this ); }, 0, 255 );
	effectRainbowStepNE.size = 4;
	addString( box4, 'Rainbow step', effectRainbowStepNE );
	let effectFireSpeedNE = createEditorElement( 'number', '', 'fire_speed', function(){ changeEffect( this ); }, 0, 255 );
	effectFireSpeedNE.size = 4;
	addString( box4, 'Fire speed', effectFireSpeedNE );
	let effectFireStepNE = createEditorElement( 'number', '', 'fire_step', function(){ changeEffect( this ); }, 0, 255 );
	effectFireStepNE.size = 4;
	addString( box4, 'Fire step', effectFireStepNE );
	let effectPulseSpeedNE = createEditorElement( 'number', '', 'pulse_speed', function(){ changeEffect( this ); }, 0, 255 );
	effectPulseSpeedNE.size = 4;
	addString( box4, 'Pulse speed', effectPulseSpeedNE );
	let effectPulseStepNE = createEditorElement( 'number', '', 'pulse_step', function(){ changeEffect( this ); }, 0, 255 );
	effectPulseStepNE.size = 4;
	addString( box4, 'Pulse step', effectPulseStepNE );
	



	let buttonBox = document.createElement( 'div' );
	buttonBox.classList.add( 'flex' );
	obj.appendChild( buttonBox );

	let sb = document.createElement( 'button' );
	sb.name = 'action';
	sb.value = 'save';
	sb.innerHTML = '<text>Сохранить</text>';
	sb.onclick = function(){ changeParam( this ); };
	buttonBox.appendChild( sb );

	let rb = document.createElement( 'button' );
	rb.name = 'action';
	rb.value = 'reboot';
	rb.innerHTML = '<text>Перезагрузить</text>';
	rb.onclick = function(){ changeParam( this ); setTimeout(() => document.location.reload(), 3000 ); };
	buttonBox.appendChild( rb );
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
function rebuildInputsOutsSettings()
{
	let obj = document.getElementById( 'ioConfigureBlock' );
	if( obj == undefined ) return;

	obj.innerHTML = '';

	for( let i = 0; i < app.data.portsMax; i++ ){
		let use = CheckBit( app.data.use_ports, i );
		let leds = app.data.leds[ i ];
		let checked = ( use ) ? ' checked' : '';
		let disabled = ( !Number( use ) ) ? ' disabled' : '';
		let portName = 'Port ' + ( i + 1 );

		let div = document.createElement( 'div' );
		div.className = 'flex';
		div.innerHTML = '<input type="checkbox" name="u_port" lang="' + i + '" onchange="changeState( this );"' + checked + '><input type="number" min="0" max="9999" name="port" lang="' + i + '" onchange="changePortSize( this );" value="' + leds + '" size="4"' + disabled + '>';

		addString( obj, portName, div );
	}

	for( let i = 0; i < app.data.outsMax; i++ ){
		let use = CheckBit( app.data.use_outs, i );
		let state = CheckBit( app.data.outsState, i );
		let outName = 'Out ' + ( i + 1 );

		let checkedU = ( Number( use ) ) ? ' checked' : '';
		let checkedS = ( Number( state ) ) ? ' checked' : '';
		let disabled = ( !Number( use ) ) ? ' disabled' : '';


		let div = document.createElement( 'div' );
		div.className = 'flex';
		div.innerHTML = '<input type="checkbox" name="u_out" lang="' + i + '" onchange="changeState( this );"' + checkedU + '><input type="checkbox" name="out" lang="' + i + '" onchange="changeState( this );"' + checkedS + disabled + '>';

		addString( obj, outName, div );
	}
}

//-----------------------------------------------------------------------------
function rebuildZonesSettings()
{
	let obj = document.getElementById( 'zonesConfigureBlock' );
	if( obj == undefined ) return;

	obj.innerHTML = '';

	for( let i = 0; i < app.data.zonesCount; i++ ){
		if( !app.data.zones.hasOwnProperty( i ) ) continue;
		let port						= app.data.zones[ i ].port;
		let start						= app.data.zones[ i ].start;
		let count						= app.data.zones[ i ].count;
		let name						= 'Zone ' + ( i + 1 );

		let div = document.createElement( 'div' );
		div.className = 'flex';
		div.innerHTML = '<input type="number" min="0" max="8" name="portAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + port + '" size="3"><input type="number" min="0" max="9999" name="startAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + start + '" size="5"><input type="number" min="0" max="9999" name="countAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + count + '" size="5">';

		addString( obj, name, div );
	}
}

//-----------------------------------------------------------------------------
function changeParam( element, valueLen = 0, value = undefined )
{
	if( app.debug ) console.log( 'changeParam >:', element.name, element.value, valueLen, value );

	if( element.type == 'checkbox' ){
		value = ( element.checked ) ? 1 : 0;
		valueLen = 1;
	}else if( element.tagName == 'SELECT' ){
		valueLen = 1;
	}else if( element.type == 'color' ){
		value = hexToRgb( element.value );
		// value = ( ( rgb.r << 24 ) | ( rgb.g << 16 ) | ( rgb.b << 8 ) | 0 );
		// value >>>= 0;
		valueLen = 3;
	}else if( element.tagName == 'BUTTON' ){
		valueLen = 1;
		switch( element.value ){
			case 'reboot':			value = 0x00;		break;
			case 'saveReboot':		value = 0x01;		break;
			case 'save':			value = 0x02;		break;
		}
	}else if( value == undefined ){
		value = [];
		if( valueLen == 1 ){
			value.push( element.value );
		}else if( valueLen == 2 ){
			value.push( element.value >> 8 );
			value.push( element.value );
		}else if( valueLen == 3 ){
			value.push( element.value >> 16 );
			value.push( element.value >> 8 );
			value.push( element.value );
		}else if( valueLen == 4 ){
			value.push( element.value >> 24 );
			value.push( element.value >> 16 );
			value.push( element.value >> 8 );
			value.push( element.value );
		}
	}

	let paramID = -1;
	for( let indx in app.paramIDs ){
		if( app.paramIDs[ indx ] == element.name ){
			paramID = indx;
			break;
		}
	}

	let offset = 0;
	let binary = new Uint8Array( 2 + valueLen );
	// Command for set parametr
	binary[ offset++ ] = 0xF2;
	binary[ offset++ ] = paramID;
	if( element.type == 'color' ){
		binary[ offset++ ] = value.r;
		binary[ offset++ ] = value.g;
		binary[ offset++ ] = value.b;
	}else if( element.tagName == 'SELECT' ){
		binary[ offset++ ] = element.value;
	}else if( element.tagName == 'BUTTON' ){
		binary[ offset++ ] = value;
	}else{
		for( let i = 0; i < valueLen; i++ ){
			binary[ offset++ ] = value[ i ];
		}
	}

	console.log( binary, element.tagName );
	
	app.ws.send( binary.buffer );
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
				if( dataObject.hasOwnProperty( 'masterColor' ) ) app.data.masterColor = dataObject.masterColor;

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
function addHeader( obj = undefined, title = '' )
{
	if( obj == undefined ) return;

	let header = document.createElement( 'div' );
	header.classList = 'header';
	header.innerHTML = title;
	obj.appendChild( header );
}

//-----------------------------------------------------------------------------
function addString( obj = undefined, title = '', controlObject )
{
	if( obj == undefined ) return;

	let titleBox = document.createElement( 'text' );
	titleBox.innerText = title;

	let modeString = document.createElement( 'div' );
	modeString.classList = 'string';
	modeString.appendChild( titleBox );
	modeString.appendChild( controlObject );

	obj.appendChild( modeString );
}

//-----------------------------------------------------------------------------
function setConnectionStatus( status, className = '' )
{
	let element = document.getElementById( 'conStatL' );
	if( element == undefined ) return;
	element.innerHTML = status;
	if( className != '' ) element.className = className;
}

//-----------------------------------------------------------------------------
function setValueFromName( name, value )
{
	let list = document.getElementsByName( name );
	for( let i = 0; i < list.length; i++ ) list[ i ].value = value;
}

//-----------------------------------------------------------------------------
function getValueFromName( name )
{
	let list = document.getElementsByName( name );
	return ( list.length > 0 ) ? list[ 0 ].value : null;
}

//-----------------------------------------------------------------------------
function saveValueToStorage( name, value )
{
	app.localStorage.saveToStorage( 'settings', name, value, '{}' );
}

//-----------------------------------------------------------------------------
function changeState( element )
{
	if( element.nextElementSibling != undefined ){
		element.nextElementSibling.disabled = ( !element.checked ) ? true : false;
	}

	let bit = Number( element.lang );
	let obj = document.createElement( 'input' );
	let valueLen = 0;
	obj.type = 'number';

	if( element.name == 'u_port' ){
		app.data.use_ports = setBit( app.data.use_ports, bit, element.checked );
		obj.value = app.data.use_ports;
		valueLen = 1;
	}else if( element.name == 'u_out' ){
		app.data.use_outs = setBit( app.data.use_outs, bit, element.checked );
		obj.value = app.data.use_outs;
		valueLen = 1;
	}else if( element.name == 'out' ){
		app.data.outsState = setBit( app.data.outsState, bit, element.checked );
		obj.value = app.data.outsState;
		valueLen = 1;
	}

	obj.name = element.name;
	
	changeParam( obj, valueLen );
}

//-----------------------------------------------------------------------------
function changeZone( element )
{
	let zone = Number( element.lang );
	let zoneData = app.data.zones[ zone ];
	if( zoneData == undefined ) return;

	let obj = document.createElement( 'input' );
	obj.name = 'zone';
	obj.type = 'number';

	if( element.name == 'portAtZone' ){
		zoneData.port = element.value;
	}else if( element.name == 'startAtZone' ){
		zoneData.start = element.value;
	}else if( element.name == 'countAtZone' ){
		zoneData.count = element.value;
	}

	let data = new Uint8Array( 6 );

	data[ 0 ] = zone;
	data[ 1 ] = zoneData.port;
	data[ 2 ] = zoneData.start >> 8;
	data[ 3 ] = zoneData.start;
	data[ 4 ] = zoneData.count >> 8;
	data[ 5 ] = zoneData.count;
	
	changeParam( obj, data.length, data );
}

//-----------------------------------------------------------------------------
function changeEffect( element )
{
	let value = element.value;

	let obj = document.createElement( 'input' );
	obj.name = 'effect';
	obj.type = 'number';

	if( element.name == 'rainbow_speed' ){
		app.data.effects.rainbow_speed = element.value;
	}else if( element.name == 'rainbow_step' ){
		app.data.effects.rainbow_step = element.value;
	}

	let effectID = 0xFF;
	for( let indx in app.effectIDs ){
		if( app.effectIDs[ indx ] == element.name ){
			effectID = indx;
			break;
		}
	}

	let data = new Uint8Array( 3 );

	data[ 0 ] = effectID;
	data[ 1 ] = value >> 8;
	data[ 2 ] = value;
	
	changeParam( obj, data.length, data );
}

//-----------------------------------------------------------------------------
function changePortSize( element )
{
	let value = element.value;
	let portNum = Number( element.lang );

	let obj = document.createElement( 'input' );
	obj.name = 'port';
	obj.type = 'number';

	app.data.leds[ portNum ] = value;

	let data = new Uint8Array( 3 );

	data[ 0 ] = portNum;
	data[ 1 ] = value >> 8;
	data[ 2 ] = value;
	
	changeParam( obj, data.length, data );
}

//-----------------------------------------------------------------------------
function createEditorElement( type = '', className = '',  name = '', onchange = undefined, min = undefined, max = undefined )
{
	let element = undefined;

	if( type == 'select' ){
		element = document.createElement( 'select' );
	}else{
		element = document.createElement( 'input' );
		element.type			= type;
	}
	
	element.name			= name;

	if( className != '' ) element.className = className;
	if( onchange != undefined ) element.onchange = onchange;
	if( min != undefined ) element.min = min;
	if( max != undefined ) element.max = max;

	return element;
}

//-----------------------------------------------------------------------------
