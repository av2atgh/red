// functions

void free_dinet(struct dinet_class *dinet) {
	free((*dinet).node);
	free((*dinet).arc);
	free((*dinet).ordering);
	free((*dinet).list);
	return;
}

