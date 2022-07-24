#include "plugin.hpp"


struct EdoQuant : Module {
	enum ParamId {
		EDO_PARAM,
		RESTRICT_PARAM,
		OFFSET_PARAM,
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

	EdoQuant() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configParam(EDO_PARAM, 1, 24, 12, "");
		configParam(RESTRICT_PARAM, 1, 13, 1, "");
		configParam(OFFSET_PARAM, -0.5, 0.5, 0.f, "");
		configInput(INPUT_INPUT, "");
		configOutput(OUTPUT_OUTPUT, "");
	}

	void process(const ProcessArgs& args) override {
    float inputVoltage = inputs[INPUT_INPUT].getVoltage();

    // 1edo to 24edo
    int edo = floor(params[EDO_PARAM].getValue());
    int divisor = floor(params[RESTRICT_PARAM].getValue());
    int offset = params[OFFSET_PARAM].getValue();

    float outputVoltage = getVoltage(inputVoltage, edo, divisor, offset);
    outputs[OUTPUT_OUTPUT].setVoltage(outputVoltage);

	}

  float getVoltage(float input, int divisions, int levels, float offset) {
    float base = floor(input) + (float)floor(offset * divisions) / divisions;
    float difference = (float)floor( (float)floor ((input - base) * divisions) / levels) * levels / divisions;

    float output = base + difference;
    return output;

  }

};


struct EdoQuantWidget : ModuleWidget {
	EdoQuantWidget(EdoQuant* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/EdoQuant.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(15.101, 42.58)), module, EdoQuant::EDO_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(15.101, 62.977)), module, EdoQuant::RESTRICT_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.101, 83.374)), module, EdoQuant::OFFSET_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.101, 22.184)), module, EdoQuant::INPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.101, 103.771)), module, EdoQuant::OUTPUT_OUTPUT));
	}
};


Model* modelEdoQuant = createModel<EdoQuant, EdoQuantWidget>("EdoQuant");