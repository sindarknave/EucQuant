#include "plugin.hpp"


struct EucQuant : Module {
	enum ParamId {
		DIVISIONS_PARAM,
		LEVELS_PARAM,
		ROTATION_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUT_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	EucQuant() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(DIVISIONS_PARAM, 1, 24, 12, "");
		configParam(LEVELS_PARAM, 1, 4, 1, "");
		configParam(ROTATION_PARAM, -0.5, 0.5, 0.f, "");
		configInput(INPUT_INPUT, "");
		configOutput(OUTPUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
    float inputVoltage = inputs[INPUT_INPUT].getVoltage();

    // 1edo to 24edo
/*     int divisions = 1 + floor(params[DIVISIONS_PARAM].getValue() * 23);
    int levels = 1 + floor(params[LEVELS_PARAM].getValue() * 4);
    int rotation = params[ROTATION_PARAM].getValue();

 */    
    int divisions = floor(params[DIVISIONS_PARAM].getValue());
    int levels = floor(params[LEVELS_PARAM].getValue());
    int rotation = params[ROTATION_PARAM].getValue();

    float outputVoltage = getVoltage(inputVoltage, divisions, levels, rotation);

    //float outputVoltage = (float) baseVoltage + (float) floor((inputVoltage - baseVoltage)/ (1/divisions))/(float)divisions;

    outputs[OUTPUT_OUTPUT].setVoltage(outputVoltage);
	}

  float getVoltage(float input, int divisions, int levels, float rotation) {
    float base = floor(input) + (float)floor(rotation * divisions) / divisions;
    float difference = (float)floor( (float)floor ((input - floor(input)) * divisions) / levels) * levels / divisions;

    float output = base + difference;
    return output;

  }
};


struct EucQuantWidget : ModuleWidget {
	EucQuantWidget(struct EucQuant* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/EucQuant.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 41.928)), module, EucQuant::DIVISIONS_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.519, 62.117)), module, EucQuant::LEVELS_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.713, 83.711)), module, EucQuant::ROTATION_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.033, 22.184)), module, EucQuant::INPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 105.405)), module, EucQuant::OUTPUT_OUTPUT));
	}
};


Model* modelEucQuant = createModel<EucQuant, EucQuantWidget>("EucQuant");