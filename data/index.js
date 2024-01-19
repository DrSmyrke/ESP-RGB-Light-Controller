//-----------------------------------------------------------------------------
var app = {
	debug: true,
	error: false,
	actionProcess: false,
	url: undefined,
	SVGImages: undefined,
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
			rainbow_anim_delay: 0,
			rainbow_speed: 0,
			rainbow_step: 0,
			rainbow_orders: [],
			fire_anim_delay: 0,
			fire_speed: 0,
			fire_step: 0,
			fire_hue_gap: 0,
			fire_hue_start: 0,
			fire_min_bright: 0,
			fire_max_bright: 0,
			fire_min_sat: 0,
			fire_max_sat: 0,
			pulse_anim_delay: 0,
			pulse_speed: 0,
			pulse_step: 0,
			pulse_orders: [],
			masterColor: { r: 0, g: 0, b:0 },
			zoneColors: {},
		},
	},
	modesAvailable: [
		{ name: 'Inpulse', ico: '<svg fill="#000000" version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="800px" height="800px" viewBox="0 0 470.34 470.34" xml:space="preserve"><g><path d="M235.169,470.34c99.262,0,180.016-80.756,180.016-180.018c0-41.055-28.979-101.928-86.129-180.933 C287.262,51.617,244.866,4.585,244.442,4.117C242.072,1.496,238.705,0,235.17,0c-3.534,0-6.903,1.496-9.273,4.117 c-0.423,0.469-42.819,47.5-84.613,105.272c-57.152,79.004-86.13,139.878-86.13,180.933 C55.152,389.584,135.907,470.34,235.169,470.34z M161.41,124.223c29.423-40.688,59.156-75.953,73.761-92.8 c14.578,16.813,44.227,51.977,73.628,92.621c53.244,73.6,81.387,131.099,81.387,166.278c0,85.477-69.541,155.018-155.017,155.018 c-85.477,0-155.017-69.541-155.017-155.018C80.152,255.173,108.251,197.736,161.41,124.223z"/></g></svg>' },
		{ name: 'Rainbow', ico: '<svg fill="#000000" width="800px" height="800px" viewBox="0 0 256 256" id="Flat" xmlns="http://www.w3.org/2000/svg"><path d="M184,168v16a8,8,0,0,1-16,0V168a40,40,0,0,0-80,0v16a8,8,0,0,1-16,0V168a56,56,0,0,1,112,0ZM128,80a88.09957,88.09957,0,0,0-88,88v16a8,8,0,0,0,16,0V168a72,72,0,0,1,144,0v16a8,8,0,0,0,16,0V168A88.09957,88.09957,0,0,0,128,80Zm0-32A120.13623,120.13623,0,0,0,8,168v16a8,8,0,0,0,16,0V168a104,104,0,0,1,208,0v16a8,8,0,0,0,16,0V168A120.13623,120.13623,0,0,0,128,48Z"/> </svg>' },
		{ name: 'Fire', ico: '<svg fill="#000000" width="800px" height="800px" viewBox="0 0 32 32" version="1.1" xmlns="http://www.w3.org/2000/svg"><path d="M15.888 31.977c-7.539 0-12.887-5.228-12.887-12.431 0-3.824 2.293-7.944 2.39-8.116 0.199-0.354 0.59-0.547 0.998-0.502 0.404 0.052 0.736 0.343 0.84 0.736 0.006 0.024 0.624 2.336 1.44 3.62 0.548 0.864 1.104 1.475 1.729 1.899-0.423-1.833-0.747-4.591-0.22-7.421 1.448-7.768 7.562-9.627 7.824-9.701 0.337-0.097 0.695-0.010 0.951 0.223 0.256 0.235 0.373 0.586 0.307 0.927-0.010 0.054-1.020 5.493 1.123 10.127 0.195 0.421 0.466 0.91 0.758 1.399 0.083-0.672 0.212-1.386 0.41-2.080 0.786-2.749 2.819-3.688 2.904-3.726 0.339-0.154 0.735-0.104 1.027 0.126 0.292 0.231 0.433 0.603 0.365 0.969-0.011 0.068-0.294 1.938 1.298 4.592 1.438 2.396 1.852 3.949 1.852 6.928 0 7.203-5.514 12.43-13.111 12.43zM6.115 14.615c-0.549 1.385-1.115 3.226-1.115 4.931 0 6.044 4.506 10.43 10.887 10.43 6.438 0 11.11-4.386 11.11-10.431 0-2.611-0.323-3.822-1.567-5.899-0.832-1.386-1.243-2.633-1.439-3.625-0.198 0.321-0.382 0.712-0.516 1.184-0.61 2.131-0.456 4.623-0.454 4.649 0.029 0.446-0.242 0.859-0.664 1.008s-0.892 0.002-1.151-0.364c-0.075-0.107-1.854-2.624-2.637-4.32-1.628-3.518-1.601-7.323-1.434-9.514-1.648 0.96-4.177 3.104-4.989 7.466-0.791 4.244 0.746 8.488 0.762 8.529 0.133 0.346 0.063 0.739-0.181 1.018-0.245 0.277-0.622 0.4-0.986 0.313-0.124-0.030-2.938-0.762-4.761-3.634-0.325-0.514-0.617-1.137-0.864-1.742z"></path></svg>' },
		{ name: 'One Color', ico: '<svg width="800px" height="800px" viewBox="0 0 16 16" xmlns="http://www.w3.org/2000/svg"><defs><linearGradient id="a" x1="814.98" x2="881.02" y1="235.19" y2="235.19" gradientTransform="matrix(1.3745 0 0 1.3633 -317.33 -85.443)" gradientUnits="userSpaceOnUse"><stop stop-color="#8a8a8a" offset="0"/><stop stop-color="#f9f9f9" offset="1"/></linearGradient></defs><path d="M878.98 235.19c.012 17.094-13.851 30.957-30.957 30.957s-30.969-13.863-30.957-30.957c-.012-17.094 13.851-30.957 30.957-30.957s30.969 13.863 30.957 30.957z" color="#000000" fill="url(#a)" stroke="#373737" stroke-width="5.6502" transform="matrix(.17626 0 0 .17771 -141.52 -33.795)"/></svg>' },
		{ name: 'Color Zones', ico: '<svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 512 512" xml:space="preserve"><rect x="346.493" y="148.276" transform="matrix(0.7071 0.7071 -0.7071 0.7071 258.7865 -241.3502)" style="fill:#F23488;" width="148.471" height="86.865"/><polygon style="fill:#23CE9E;" points="182.885,80.502 331.346,80.509 331.346,167.439 182.891,167.439 "/><polygon style="fill:#CE246E;" points="337.517,169.929 442.498,274.911 381.08,336.33 331.339,286.59 331.339,254.308 331.346,176.099 "/><polygon style="fill:#1AB281;" points="331.346,176.099 331.339,254.308 261.086,254.308 232.548,225.77 182.885,176.107 182.891,167.439 331.346,167.439 "/><path style="fill:#129665;" d="M331.339,254.308v32.282l0.006,52.327c-2.901-12.006-9.036-23.385-18.407-32.755l-18.972-18.972 l-32.881-32.881L331.339,254.308L331.339,254.308z"/><path style="fill:#AF1A5B;" d="M331.339,286.59l49.741,49.741l-49.741,49.741v-14.335c2.608-10.766,2.614-22.039,0.006-32.819 L331.339,286.59z"/><path style="fill:#E4EAEA;" d="M293.968,287.189l18.972,18.972c9.371,9.371,15.506,20.75,18.407,32.755l-0.006,32.819 c-2.907,11.999-9.036,23.371-18.4,32.735l-5.571,5.571l-1.101,1.101c-27.143,27.143-71.159,27.136-98.303-0.006l-18.972-18.972 L293.968,287.189z"/><polygon style="fill:#AD6102;" points="261.086,254.308 293.968,287.189 188.993,392.164 127.573,330.745 232.548,225.77 "/><polygon style="fill:#D1800D;" points="182.885,176.107 232.548,225.77 127.573,330.745 66.147,269.319 171.122,164.344 "/><rect x="48.534" y="113.561" transform="matrix(0.7071 0.7071 -0.7071 0.7071 159.0308 -8.3528)" style="fill:#EA9B22;" width="82.127" height="148.46"/><circle style="fill:#7A8280;" cx="272.429" cy="373.796" r="19.152"/><path d="M318.649,300.451L176.836,158.638c-0.001-0.001-0.002-0.002-0.003-0.004s-0.002-0.002-0.004-0.003l-58.069-58.069 c-3.154-3.154-8.267-3.154-11.419,0l-30.214,30.214c-3.154,3.154-3.154,8.267,0,11.419c3.154,3.154,8.267,3.154,11.419,0 l24.505-24.504l46.653,46.653l-93.556,93.555l-46.653-46.653l51.046-51.045c3.154-3.154,3.154-8.267,0-11.419 c-3.154-3.155-8.267-3.154-11.419,0L2.365,205.537C0.851,207.051,0,209.105,0,211.247c0,2.141,0.851,4.196,2.365,5.71l199.89,199.89 c14.652,14.652,34.136,22.723,54.86,22.725c0.003,0,0.004,0,0.008,0c20.721,0,40.204-8.069,54.854-22.719l6.673-6.672 c10.088-10.088,17.19-22.725,20.537-36.543c2.903-11.98,2.905-24.643,0.009-36.616C335.855,323.199,328.751,310.553,318.649,300.451 z M171.122,175.764l50.007,50.007l-61.352,61.351c-3.154,3.154-3.154,8.267,0,11.419c1.576,1.576,3.643,2.365,5.71,2.365 s4.133-0.788,5.71-2.365l61.352-61.352l49.999,49.999l-93.556,93.556l-49.999-49.999l12.948-12.948 c3.154-3.154,3.154-8.267,0-11.419c-3.154-3.154-8.267-3.154-11.419,0l-12.948,12.948l-50.007-50.007L171.122,175.764z	M323.491,369.833c-2.649,10.935-8.273,20.937-16.262,28.927l-6.673,6.673c-11.6,11.6-27.025,17.989-43.434,17.989 c-0.001,0-0.004,0-0.006,0c-16.411-0.001-31.839-6.392-43.442-17.995l-13.262-13.262l93.556-93.556l13.262,13.262 c8.001,8.002,13.626,18.009,16.268,28.945C325.794,350.306,325.791,360.339,323.491,369.833z"/><path d="M182.891,155.524c4.46,0,8.075-3.615,8.075-8.075l-0.005-58.871l132.312,0.005l-0.003,70.781H199.557 c-4.46,0-8.075,3.615-8.075,8.075s3.615,8.075,8.075,8.075h123.711l-0.003,70.718H292.21c-4.46,0-8.075,3.615-8.075,8.075 s3.615,8.075,8.075,8.075h31.054l0.001,35.966c0,4.46,3.617,8.075,8.076,8.074c4.46-0.001,8.075-3.617,8.074-8.076l0.003-130.86 c0-0.015,0.002-0.03,0.002-0.045c0-0.015-0.002-0.03-0.002-0.045l0.002-86.885c0-4.46-3.614-8.075-8.075-8.075l-148.462-0.006l0,0 c-2.141,0-4.196,0.851-5.71,2.365c-1.515,1.515-2.365,3.568-2.365,5.711l0.005,66.947 C174.816,151.91,178.432,155.524,182.891,155.524z"/> <path d="M509.635,207.775L404.652,102.793c-1.514-1.515-3.568-2.365-5.71-2.365s-4.196,0.851-5.71,2.365l-48.23,48.23 c-3.154,3.154-3.154,8.267,0,11.419c3.154,3.154,8.267,3.154,11.419,0l42.521-42.521l93.562,93.562l-2.539,2.539 c-3.154,3.154-3.154,8.267,0,11.419c1.576,1.576,3.643,2.365,5.71,2.365s4.133-0.788,5.71-2.365l8.249-8.249 c1.515-1.514,2.365-3.568,2.365-5.71C512,211.342,511.148,209.288,509.635,207.775z"/><path d="M470.203,235.787l-27.704,27.704l-86.632-86.632c-3.154-3.154-8.267-3.154-11.419,0c-3.154,3.154-3.154,8.267,0,11.419 l86.632,86.632l-49.999,49.999l-26.659-26.659c-3.154-3.154-8.267-3.154-11.419,0c-3.154,3.154-3.154,8.267,0,11.419l26.659,26.659 l-24.144,24.144c-3.154,3.154-3.154,8.267,0,11.419c1.576,1.577,3.643,2.365,5.71,2.365s4.133-0.788,5.71-2.365l124.687-124.687 c3.154-3.154,3.154-8.267,0-11.419C478.468,232.634,473.355,232.635,470.203,235.787z"/><path d="M291.679,354.544c-10.615-10.614-27.889-10.615-38.504,0c-5.142,5.142-7.975,11.979-7.975,19.252s2.832,14.11,7.975,19.252 c5.308,5.308,12.279,7.962,19.252,7.962c6.973,0,13.944-2.654,19.252-7.962C302.293,382.432,302.293,365.159,291.679,354.544 C291.679,354.545,291.679,354.545,291.679,354.544z M280.258,381.628c-4.318,4.318-11.346,4.318-15.664,0 c-2.092-2.092-3.244-4.873-3.244-7.832c0-2.959,1.152-5.741,3.244-7.832c2.159-2.16,4.996-3.239,7.832-3.239 c2.837,0,5.674,1.079,7.832,3.239C284.577,370.282,284.577,377.31,280.258,381.628z"/></svg>' },
	],
	paramIDs: [
		'mode', 'u_port', 'u_out', 'out', 'action', 'zone', 'effect', 'port'
	],
	effectIDs: [
		'rainbow_anim_delay', 'rainbow_speed', 'rainbow_step', 'rainbow_orders', 'fire_anim_delay', 'fire_speed', 'fire_step', 'fire_hue_gap', 'fire_hue_start', 'fire_min_bright', 'fire_max_bright', 'fire_min_sat', 'fire_max_sat', 'pulse_anim_delay', 'pulse_speed', 'pulse_step', 'pulse_orders', 'masterColor', 'zoneColor', '', '', '', '', ''
	],
	ws: undefined,
	wsAddr: '',
	reconnectAttempts: 0,
	localStorage: undefined,
};

//-----------------------------------------------------------------------------
window.onload = function(){
	app.localStorage = new Storage( false );
	app.SVGImages = new SVGImages();

	printLocation();
	buildControlUI();
	buildSettingsUI();
	updateGroupsUI();
	buildModes();
	updateModes();

	let data = app.localStorage.getStorageData( 'settings', '{}' );
	//
	//app.localStorage.removeFromStorage( 'sets', indx, undefined, '[]' );

	setValueFromName( 'tHost', ( data.hasOwnProperty( 'tHost' ) ) ? data.tHost : document.location.host );
	setValueFromName( 'tPort', ( data.hasOwnProperty( 'tPort' ) ) ? data.tPort : 81 );
	wsConnect();

	// updateData( function(){ app.url = undefined; openPage( document.location.pathname ); } );
	updateMemory();

	app.SVGImages.replaceAll();
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
	// app.ws.onclose = this.deinit();
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
				let port								= view.getUint8( offset++ ); offset++;
				let start								= ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
				let count								= ( ( view.getUint8( offset + 1 ) << 8 ) | ( view.getUint8( offset ) ) ); offset += 2;
				app.data.zones[ i ] = { port: port, start: start, count: count };
				// offset++;
			}

			// Effects
			app.data.effects.rainbow_anim_delay			= view.getUint8( offset++ );
			app.data.effects.rainbow_speed				= view.getUint8( offset++ );
			app.data.effects.rainbow_step				= view.getUint8( offset++ );
			//rainbow_orders
			app.data.effects.rainbow_orders				= [];
			for( let i = 0; i < 8; i++ ){
				app.data.effects.rainbow_orders.push( view.getUint8( offset++ ) );		//Port
				app.data.effects.rainbow_orders.push( view.getUint8( offset++ ) );		//Direction
			}
			
			app.data.effects.fire_anim_delay			= view.getUint8( offset++ );
			app.data.effects.fire_speed					= view.getUint8( offset++ );
			app.data.effects.fire_step					= view.getUint8( offset++ );
			app.data.effects.fire_hue_gap				= view.getUint8( offset++ );
			app.data.effects.fire_hue_start				= view.getUint8( offset++ );
			app.data.effects.fire_min_bright			= view.getUint8( offset++ );
			app.data.effects.fire_max_bright			= view.getUint8( offset++ );
			app.data.effects.fire_min_sat				= view.getUint8( offset++ );
			app.data.effects.fire_max_sat				= view.getUint8( offset++ );
			app.data.effects.pulse_anim_delay			= view.getUint8( offset++ );
			app.data.effects.pulse_speed				= view.getUint8( offset++ );
			app.data.effects.pulse_step					= view.getUint8( offset++ );
			//pulse_orders
			app.data.effects.pulse_orders				= [];
			for( let i = 0; i < 8; i++ ){
				app.data.effects.pulse_orders.push( view.getUint8( offset++ ) );			//Port
				app.data.effects.pulse_orders.push( view.getUint8( offset++ ) );			//Direction
			}

			app.data.effects.masterColor.r				= view.getUint8( offset++ );
			app.data.effects.masterColor.g				= view.getUint8( offset++ );
			app.data.effects.masterColor.b				= view.getUint8( offset++ );

			for( let i = 0; i < app.data.zonesCount; i++ ){
				app.data.effects.zoneColors[ i ]		= { r: view.getUint8( offset++ ), g: view.getUint8( offset++ ), b: view.getUint8( offset++ ) };
			}

			rebuildInputsOutsSettings();
			rebuildZonesSettings();
			updateModes();

			setFieldValue( 'rainbow_anim_delay', app.data.effects.rainbow_anim_delay );
			setFieldValue( 'rainbow_speed', app.data.effects.rainbow_speed );
			setFieldValue( 'rainbow_step', app.data.effects.rainbow_step );
			setFieldValue( 'rainbow_orders', app.data.effects.rainbow_orders.join( ',' ) );
			setFieldValue( 'fire_anim_delay', app.data.effects.fire_anim_delay );
			setFieldValue( 'fire_speed', app.data.effects.fire_speed );
			setFieldValue( 'fire_step', app.data.effects.fire_step );
			setFieldValue( 'fire_hue_gap', app.data.effects.fire_hue_gap );
			setFieldValue( 'fire_hue_start', app.data.effects.fire_hue_start );
			setFieldValue( 'fire_min_bright', app.data.effects.fire_min_bright );
			setFieldValue( 'fire_max_bright', app.data.effects.fire_max_bright );
			setFieldValue( 'fire_min_sat', app.data.effects.fire_min_sat );
			setFieldValue( 'fire_max_sat', app.data.effects.fire_max_sat );
			setFieldValue( 'pulse_anim_delay', app.data.effects.pulse_anim_delay );
			setFieldValue( 'pulse_speed', app.data.effects.pulse_speed );
			setFieldValue( 'pulse_step', app.data.effects.pulse_step );
			setFieldValue( 'pulse_orders', app.data.effects.pulse_orders.join( ',' ) );
			setFieldValue( 'masterColor', '#' + rgbToHex( app.data.effects.masterColor ) );

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
		}else if( command == 0xA2 ){
			// Device memory status
			let offset = 1;

			// System state
			let freeMemory								= ( ( view.getUint8( offset++ ) << 24 ) | ( view.getUint8( offset++ ) << 16 ) | ( view.getUint8( offset++ ) << 8 ) | ( view.getUint8( offset++ ) ) );
			let totalMemory								= ( ( view.getUint8( offset++ ) << 24 ) | ( view.getUint8( offset++ ) << 16 ) | ( view.getUint8( offset++ ) << 8 ) | ( view.getUint8( offset++ ) ) );
			let obj										= document.getElementById( 'memoryStatus' );
			if( obj != undefined ) obj.innerText		= freeMemory + ' / ' + totalMemory;
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
	let zb				= document.getElementById( 'zonesGroupsBox' );
	let eb				= document.getElementById( 'effectsBox' );

	if( cb != undefined ) cb.classList.add( 'hidden' );
	if( sb != undefined ) sb.classList.add( 'hidden' );
	if( zb != undefined ) zb.classList.add( 'hidden' );
	if( eb != undefined ) eb.classList.add( 'hidden' );

	switch( url ){
		case '/control':
			title = 'Control';
			if( cb != undefined ) cb.classList.remove( 'hidden' );
		break;

		case '/settings':
			title = 'Settings';
			if( sb != undefined ) sb.classList.remove( 'hidden' );
		break;

		case '/zones':
			title = 'Zones';
			if( zb != undefined ) zb.classList.remove( 'hidden' );
		break;

		case '/effects':
			title = 'Effects';
			if( eb != undefined ) eb.classList.remove( 'hidden' );
		break;
	}

	app.url = url;

	if( title != '' ){
		// window.history.pushState( {param: 'Value'}, title, url );
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
function buildModes()
{
	let obj = document.getElementById( 'modesBox' );
	if( obj == undefined ) return console.error( 'modesBox is undefined' );
	obj.innerHTML = '';

	for( let indx in app.modesAvailable ){
		let mode = app.modesAvailable[ indx ];
		let button = document.createElement( 'button' );
		button.lang = indx;
		button.name = 'mode';
		button.value = 'mode';
		button.className = 'button mode flex column';
		button.innerHTML = mode.ico + '<text>' + mode.name + '</text>';
		button.onclick = function(){
			changeParam( this );
			app.data.mode = this.lang;
			updateModes();
		};
		obj.appendChild( button );
	}
}

//-----------------------------------------------------------------------------
function updateModes()
{
	let mb = document.getElementById( 'modesBox' );
	if( mb != undefined ){
		for( let i = 0; i < mb.children.length; i++ ){
			let button = mb.children[ i ];
			if( button.lang == app.data.mode ){
				button.classList.add( 'selected' );
			}else{
				button.classList.remove( 'selected' );
			}
		}
	}

	let cb = document.getElementById( 'controlBox' );
	if( cb != undefined ){
		for( let i = 0; i < cb.lastElementChild.children.length; i++ ){
			let box = cb.lastElementChild.children[ i ];
			if( box.lang == app.data.mode ){
				box.classList.remove( 'hidden' );
			}else{
				box.classList.add( 'hidden' );
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
		for( let portNum in app.data.ports ){
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

	rebuildInputsOutsSettings();
	rebuildZonesSettings();
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
		let humanityPort = i + 1;
		let portName = 'Port ' + humanityPort;

		let div = document.createElement( 'div' );
		div.className = 'flex';
		div.innerHTML = '<input type="checkbox" name="u_port" lang="' + i + '" onchange="changeState( this );"' + checked + '><input type="number" min="0" max="9999" name="port" lang="' + humanityPort + '" onchange="changePortSize( this );" value="' + leds + '" size="4"' + disabled + '>';

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
	let zb = document.getElementById( 'zonesBox' );
	if( zb == undefined ) return;

	zb.innerHTML = '';

	for( let i = 0; i < app.data.zonesCount; i++ ){
		if( !app.data.zones.hasOwnProperty( i ) ) continue;
		let port						= app.data.zones[ i ].port;
		let start						= app.data.zones[ i ].start;
		let count						= app.data.zones[ i ].count;
		let color						= app.data.effects.zoneColors[ i ];
		let name						= '# ' + ( i + 1 );

		if( color == undefined ) color = { r: 0, g: 0, b: 0 };

		let div = document.createElement( 'div' );
		div.className = 'flex';
		div.innerHTML = '<input type="number" min="0" max="8" name="portAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + port + '" size="3"><input type="number" min="0" max="9999" name="startAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + start + '" size="5"><input type="number" min="0" max="9999" name="countAtZone" lang="' + i + '" onchange="changeZone( this );" value="' + count + '" size="5"><input type="color" name="zoneColor" value="#' + rgbToHex( color ) + '" lang="' + i + '" onChange="changeEffect( this );">';

		addString( zb, name, div );
	}
}

//-----------------------------------------------------------------------------
function updateGroupsUI()
{
	let gb = document.getElementById( 'groupsBox' );
	if( gb == undefined ) return;

	gb.innerHTML = '';

	let data = app.localStorage.getStorageData( 'groups', '{}' );
	for( let name in data ){
		let zones = data[ name ];
		let string = document.createElement( 'div' );
		string.className = 'string';
		let text = document.createElement( 'text' );
		text.innerText = name;
		string.appendChild( text );
		let div = document.createElement( 'div' );
		div.className = 'flex';

		let removeIco = app.SVGImages.getSvgFromImageName( 'trash' );
		removeIco.classList.add( 'button' );
		removeIco.classList.add( 'ico' );
		removeIco.classList.add( 'nobg' );
		removeIco.lang = name;
		removeIco.onclick = function(){ removeGroup( this ); };
		div.appendChild( removeIco );

		div.innerHTML += ' | <input type="color" name="groupColor" lang="' + name + '" onChange="setGroupColor( this );">';
		string.appendChild( div );
		gb.appendChild( string );
	}


	//Делаем добавлялку группы
	let div = document.createElement( 'div' );
	div.className = 'string';
	div.innerHTML = '<input type="text" placeholder="Zone id,Zone id,..." name="newZone"><input type="button" value="Create group" onClick="newGroup();">';
	gb.appendChild( div );
}

//-----------------------------------------------------------------------------
function removeGroup( element )
{
	if( element == undefined ) return;
	if( app.debug ) console.log( 'removeGroup >:', element );
	let groupName = element.lang;

	if( !confirm( 'Are you sure to delete group: ' + groupName + '?' ) ) return;

	app.localStorage.removeFromStorage( 'groups', groupName );
	updateGroupsUI();
}

//-----------------------------------------------------------------------------
function setGroupColor( element )
{
	if( app.debug ) console.log( 'setGroupColor >:', element );

	if( element == undefined ) return;
	let groupName = element.lang;
	let data = app.localStorage.getStorageData( 'groups', '{}' );
	let groupData = data[ groupName ];
	if( groupData == undefined ) return;

	for( let indx in groupData ){
		let zone = Number( groupData[ indx ] );
		let ie = document.createElement( 'input' );
		ie.type = 'color';
		ie.name = 'zoneColor';
		ie.value = element.value;
		ie.lang = zone;
		changeEffect( ie );
		ie.remove();

		app.data.effects.zoneColors[ zone ] = hexToRgb( element.value );
		updateGroupsUI();
	}
}

//-----------------------------------------------------------------------------
function newGroup()
{
	if( app.debug ) console.log( 'newGroup >:' );

	let zones = getValueFromName( 'newZone' ).split( ',' );
	if( zones.length == 0 ) return;
	let name = prompt( 'Please enter group name', '' );
	if( name != '' && name != null ){
		for( let indx in zones ){
			let zone = Number( zones[ indx ] ) - 1;
			if( zone < 0 ) zone = 0;
			zones[ indx ] = zone;
		}
		app.localStorage.saveToStorage( 'groups', name, zones );
		updateGroupsUI();
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
			case 'reboot':			value = 0x00;				break;
			case 'saveReboot':		value = 0x01;				break;
			case 'save':			value = 0x02;				break;
			case 'mode':			value = element.lang;		break;
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
function updateMemory()
{
	setTimeout( updateMemory, 250 );
	if( app.ws.readyState != 1 ) return;

	let binary = new Uint8Array( 1 );
	// Command for get free memory
	binary[ 0 ] = 0xF3;
	app.ws.send( binary.buffer );
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
	let tmpValue = new Uint8Array( 1 );
	let value = [];

	let obj = document.createElement( 'input' );
	obj.name = 'effect';
	obj.type = 'number';

	if( element.name == 'rainbow_anim_delay' ){
		app.data.effects.rainbow_anim_delay = element.value;
		value.push( element.value );
	}else if( element.name == 'rainbow_speed' ){
		app.data.effects.rainbow_speed = element.value;
		value.push( element.value );
	}else if( element.name == 'rainbow_step' ){
		app.data.effects.rainbow_step = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_anim_delay' ){
		app.data.effects.fire_anim_delay = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_speed' ){
		app.data.effects.fire_speed = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_step' ){
		app.data.effects.fire_step = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_hue_gap' ){
		app.data.effects.fire_hue_gap = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_hue_start' ){
		app.data.effects.fire_hue_start = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_min_bright' ){
		app.data.effects.fire_min_bright = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_max_bright' ){
		app.data.effects.fire_max_bright = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_min_sat' ){
		app.data.effects.fire_min_sat = element.value;
		value.push( element.value );
	}else if( element.name == 'fire_max_sat' ){
		app.data.effects.fire_max_sat = element.value;
		value.push( element.value );
	}else if( element.name == 'pulse_anim_delay' ){
		app.data.effects.pulse_anim_delay = element.value;
		value.push( element.value );
	}else if( element.name == 'pulse_speed' ){
		app.data.effects.pulse_speed = element.value;
		value.push( element.value );
	}else if( element.name == 'pulse_step' ){
		app.data.effects.pulse_step = element.value;
		value.push( element.value );
	}else if( element.name == 'masterColor' ){
		app.data.effects.masterColor = hexToRgb( element.value );
		value.push( app.data.effects.masterColor.r );
		value.push( app.data.effects.masterColor.g );
		value.push( app.data.effects.masterColor.b );
	}else if( element.name == 'zoneColor' ){
		let rgb = hexToRgb( element.value );
		app.data.effects.zoneColors[ element.lang ] = rgb;
		value.push( element.lang );
		value.push( rgb.r );
		value.push( rgb.g );
		value.push( rgb.b );
	}else if( element.name == 'rainbow_orders' ){
		let list = element.value.split( ',' );
		if( list.length == 16 ){
			for( let i = 0; i < list.length; i++ ) value.push( list[ i ] );
		}
		app.data.effects.rainbow_orders = list;
	}else if( element.name == 'pulse_orders' ){
		let list = element.value.split( ',' );
		if( list.length == 16 ){
			for( let i = 0; i < list.length; i++ ) value.push( list[ i ] );
		}
		app.data.effects.pulse_orders = list;
	}
	
	let effectID = 0xFF;
	for( let indx in app.effectIDs ){
		if( app.effectIDs[ indx ] == element.name ){
			effectID = indx;
			break;
		}
	}

	let data = new Uint8Array( 2 + value.length );
	let offset = 0;
	data[ offset++ ] = effectID;
	data[ offset++ ] = value.length;
	for( let i = 0; i < value.length; i++ ){
		data[ offset++ ] = value[ i ];
	}
	
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
function createEditorElement( type = '', className = '', name = '', onchange = undefined, min = undefined, max = undefined )
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
