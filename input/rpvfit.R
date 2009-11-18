dyn.load("fisp.so")

.Call("rpvfit_init", list(input="LesHouches.in", output="SPheno.spc"))

.Call("rpvfit_finish")

# vim: sw=2 tw=78
